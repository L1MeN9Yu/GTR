//
//  mcf_easy.c
//  GTR
//
//  Created L1MeN9Yu on 2018/9/1.
//  Copyright © 2018 limengyu.top. All rights reserved.
//

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <zconf.h>
#include <assert.h>
#include "curl.h"
#include "mcf_easy.h"
#include "mcf_easy_request.h"
#include "mcf_thread_pool.h"
#include "mcf_atomic.h"
#include "mcf_easy_header_helper.h"
#include "mcf_easy_proxy.h"

#define LOG_MAX_BUF_SIZE 512

void (*mcf_easy_log_callback)(char *log_message);

/**
 * 请求的线程池
 */
static thread_pool mcf_easy_thread_pool;

/**
 * 全局唯一当前请求id
 */
static volatile unsigned int mcf_easy_request_global_task_id = 0;

static const char *global_user_agent;

static mcf_easy_proxy *global_proxy;

static void gtr_core_config_http_version(CURL *handle);

static void mcf_easy_config_http_method(CURL *handle, mcf_easy_request *request);

static void mcf_easy_config_url(CURL *handle, const char *url);

static void mcf_easy_config_accept_encoding(CURL *handle);

static void mcf_easy_config_keep_alive(CURL *handle);

static void mcf_easy_config_headers(CURL *handle, const char *user_agent, struct curl_slist *header);

static void mcf_easy_config_verify_peer(CURL *handle, bool on);

static void mcf_easy_config_signal(CURL *handle, bool is_on);

static void mcf_easy_config_time_out(CURL *handle, unsigned int time_out);

static void mcf_easy_config_write_call_back(CURL *handle, mcf_easy_request_response_data *response_data);

static void mcf_easy_config_proxy(CURL *handle);

static void mcf_easy_config_debug(CURL *handle);

static void mcf_easy_log(const char *format, ...);

//---Private Config

static int debug_func(CURL *__unused handle, curl_infotype type,
        char *data, size_t size,
        void *__unused user_p) {
    switch (type) {
        case CURLINFO_TEXT:
            mcf_easy_log("Info: %.*s", size, data);
            break;
        case CURLINFO_HEADER_IN:
            mcf_easy_log("Rx header: %.*s", size, data);
            break;
        case CURLINFO_HEADER_OUT:
            mcf_easy_log("Tx header: %.*s", size, data);
            break;
        case CURLINFO_DATA_IN:
            mcf_easy_log("Rx data: %.*s", size, data);
            break;
        case CURLINFO_DATA_OUT:
            mcf_easy_log("Tx data: %.*s", size, data);
            break;
        case CURLINFO_SSL_DATA_IN:
            mcf_easy_log("Rx SSL data: %.*s", size, data);
            break;
        case CURLINFO_SSL_DATA_OUT:
            mcf_easy_log("Tx SSL data: %.*s", size, data);
            break;
        case CURLINFO_END:
            break;
    }

    return 0;
}

/**
 * 读取回调(put|post)
 * @param ptr ptr
 * @param size size
 * @param nmemb nmemb
 * @param userp userp
 * @return size_t
 */
static size_t
read_callback(
        void *ptr,
        size_t size,
        size_t nmemb,
        void *userp) {
    mcf_easy_request_request_data *request_data = (mcf_easy_request_request_data *) userp;
    unsigned long bytes_read = 0;
    if (size * nmemb < 1)
        return (size_t) bytes_read;

    if (size * nmemb > request_data->size_left) {
        //This chunk can handle the whole transaction
        bytes_read = request_data->size_left;
        memcpy(ptr, request_data->data + (request_data->size - request_data->size_left), bytes_read);
        request_data->size_left = 0;
    } else {
        //This chunk can only handle part of the transaction
        bytes_read = (size * nmemb);
        memcpy(ptr, request_data->data + (request_data->size - request_data->size_left), bytes_read);
        request_data->size_left = (request_data->size_left - bytes_read);
    }
    return (size_t) bytes_read;
}

static size_t
mcf_curl_write_callback(
        void *contents,
        size_t size,
        size_t nmemb,
        void *userp) {
    size_t real_size = size * nmemb;
    mcf_easy_request_response_data *response_data = (mcf_easy_request_response_data *) userp;

    response_data->response_data = realloc(response_data->response_data, response_data->response_data_size + real_size + 1);
    if (response_data->response_data == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(response_data->response_data[response_data->response_data_size]), contents, real_size);
    response_data->response_data_size += real_size;
    response_data->response_data[response_data->response_data_size] = 0;

    return real_size;
}

static
void request(mcf_easy_request *easy_request) {

    CURL *curl_handle;
    CURLcode res;

    mcf_easy_request_response_data response_data;

    response_data.response_data = malloc(1);
    response_data.response_data_size = 0;

    curl_handle = curl_easy_init();
    {
        gtr_core_config_http_version(curl_handle);
    }

    {
        mcf_easy_config_http_method(curl_handle, easy_request);
    }

    {
        mcf_easy_config_url(curl_handle, easy_request->url);
    }

    {
        mcf_easy_config_accept_encoding(curl_handle);
    }

    {
        mcf_easy_config_keep_alive(curl_handle);
    }

    {
        mcf_easy_config_verify_peer(curl_handle, true);
    }

    {
        mcf_easy_config_time_out(curl_handle, easy_request->time_out);
    }

    {
        //如果不使用threaded resolver或者c-ares,需要no_signal=1,否则在dns解析超时情况下crash
        //详见:https://curl.haxx.se/libcurl/c/CURLOPT_NOSIGNAL.html https://curl.haxx.se/libcurl/c/threadsafe.html
        mcf_easy_config_signal(curl_handle, true);
    }

    {
        mcf_easy_config_write_call_back(curl_handle, &response_data);
    }

    {
        mcf_easy_config_proxy(curl_handle);
    }

    {
        mcf_easy_config_debug(curl_handle);
    }

    {
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, mcf_curl_write_callback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &response_data);
    }

    struct curl_slist *header = mcf_easy_add_custom_headers(easy_request->header);
    mcf_easy_config_headers(curl_handle, global_user_agent, header);

    {
        res = curl_easy_perform(curl_handle);
    }

    {
        long http_response_code = 0;
        curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_response_code);

        /* check for errors */
        if (res != CURLE_OK) {
            mcf_easy_log("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            if (easy_request->on_failed) {
                easy_request->on_failed(easy_request->task_id, http_response_code, res, curl_easy_strerror(res));
            }
        } else {
            mcf_easy_log("%lu bytes retrieved\n", response_data.response_data_size);
            if (easy_request->on_succeed) {
                easy_request->on_succeed(easy_request->task_id, http_response_code, response_data.response_data, response_data.response_data_size);
            }
        }
    }

    /* cleanup curl stuff */
    if (header) {
        curl_slist_free_all(header);
    }

    {
        curl_easy_cleanup(curl_handle);
        free(easy_request->url);
        free(easy_request);
        free(response_data.response_data);
    }
}

//--- Public
void mcf_easy_init(const char *user_agent, void *log_callback) {
    if (user_agent) {
        size_t user_agent_size = strlen(user_agent) + 1;
        global_user_agent = malloc(user_agent_size);
        memcpy((void *) global_user_agent, user_agent, user_agent_size);
    } else {
        size_t user_agent_size = strlen("top.limengyu.GTR") + 1;
        global_user_agent = malloc(user_agent_size);
        memcpy((void *) global_user_agent, "top.limengyu.GTR", user_agent_size);
    }
    mcf_easy_log_callback = log_callback;
    curl_global_init(CURL_GLOBAL_ALL);
    mcf_easy_thread_pool = thread_pool_init(10);
    thread_pool_wait(mcf_easy_thread_pool);
    mcf_easy_log("mcf : global_user_agent = %s", global_user_agent);
    mcf_easy_log("curl version : %s", curl_version());
}

void __unused mcf_easy_dispose(void) {
    curl_global_cleanup();
    thread_pool_destroy(mcf_easy_thread_pool);
}

void mcf_easy_open_proxy(const char *url, unsigned int port) {
    assert(url);
    assert(port > 0);
    global_proxy = (mcf_easy_proxy *) calloc(1, sizeof(mcf_easy_proxy));
    size_t url_size = strlen(url) + 1;
    global_proxy->url = malloc(url_size);
    memcpy(global_proxy->url, url, url_size);
    global_proxy->port = port;
}

void mcf_easy_close_proxy(void) {
    if (global_proxy) {
        if (global_proxy->url) {
            free(global_proxy->url);
        }
        global_proxy = NULL;
    }
}

void mcf_easy_add_request(unsigned int *task_id, mcf_easy_request_type type, const char *url, const char *header, unsigned int time_out, const void *request_data, unsigned long request_data_size, void *succeed_callback, void *failure_callback) {
    mcf_easy_request *easy_request = (mcf_easy_request *) calloc(1, sizeof(mcf_easy_request));
    *task_id = atomic_unsigned_int_add_and_fetch(&mcf_easy_request_global_task_id, 1);
    easy_request->task_id = *task_id;
    easy_request->is_cancel = false;
    easy_request->request_type = type;
    {
        //url
        if (url) {
            size_t url_size = strlen(url) + 1;
            easy_request->url = malloc(url_size);
            memcpy(easy_request->url, url, url_size);
        }
    }
    {
        //header
        if (header) {
            size_t header_size = strlen(header) + 1;
            easy_request->header = malloc(header_size);
            memcpy(easy_request->header, header, header_size);
        }
    }
    {
        //data
        if (request_data_size > 0 && request_data != NULL) {
            easy_request->request_data = (mcf_easy_request_request_data *) calloc(1, sizeof(mcf_easy_request_request_data));
            easy_request->request_data->data = malloc((size_t) request_data_size);
            easy_request->request_data->size_left = request_data_size;
            easy_request->request_data->size = request_data_size;
            if (easy_request->request_data->data == NULL) {
                //error
            } else {
                memcpy(easy_request->request_data->data, request_data, request_data_size);
            }
        } else {
            easy_request->request_data = NULL;
        }
    }
    {
        //time out
        easy_request->time_out = time_out;
    }
    {
        //call_back
        easy_request->on_succeed = succeed_callback;
        easy_request->on_failed = failure_callback;
    }

    thread_pool_add_work(mcf_easy_thread_pool, (void *) request, easy_request);
}

//---Private Config

static
void gtr_core_config_http_version(CURL *handle) {
    curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
}

static void mcf_easy_config_http_method(CURL *handle, mcf_easy_request *request) {
    if (!request) {
        assert(request != NULL);
        return;
    }
    switch (request->request_type) {
        case mcf_easy_request_type_get:
            curl_easy_setopt(handle, CURLOPT_HTTPGET, 1L);
            break;
        case mcf_easy_request_type_post:
            curl_easy_setopt(handle, CURLOPT_POST, 1L);
            curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_callback);
            curl_easy_setopt(handle, CURLOPT_READDATA, request->request_data);
            curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, request->request_data->size);
            curl_easy_setopt(handle, CURLOPT_INFILESIZE, request->request_data->size);
            break;
        case mcf_easy_request_type_put:
            curl_easy_setopt(handle, CURLOPT_PUT, 1L);
            curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_callback);
            curl_easy_setopt(handle, CURLOPT_READDATA, request->request_data);
            break;
        case mcf_easy_request_type_download:
            curl_easy_setopt(handle, CURLOPT_HTTPGET, 1L);
            break;
        case mcf_easy_request_type_upload:
            curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
            break;
    }
}

static void mcf_easy_config_url(CURL *handle, const char *url) {
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
}

static void mcf_easy_config_accept_encoding(CURL *handle) {
    //设置CURLOPT_ACCEPT_ENCODING
    curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "");
}

static void mcf_easy_config_keep_alive(CURL *handle) {
    /* enable TCP keep-alive for this transfer */
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);
    /* keep-alive idle time to 120 seconds */
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPIDLE, 120L);
    /* interval time between keep-alive probes: 60 seconds */
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPINTVL, 60L);
}

static void mcf_easy_config_headers(CURL *handle, const char *user_agent, struct curl_slist *header) {
    curl_easy_setopt(handle, CURLOPT_USERAGENT, user_agent);
    if (header) {
        CURLcode res = curl_easy_setopt(handle, CURLOPT_HTTPHEADER, header);
        if (res != CURLE_OK) {
            fprintf(stderr,
                    "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
    }
}

static void mcf_easy_config_verify_peer(CURL *handle, bool on) {
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, on ? 1L : 0L);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, on ? 1L : 0L);
}

//TODO ETag/If_Modified_since
static void mcf_easy_config_time_condition(CURL *handle, unsigned long time) {
    /* January 1, 2020 is 1577833200 */
    curl_easy_setopt(handle, CURLOPT_TIMEVALUE, time);

    /* If-Modified-Since the above time stamp */
    curl_easy_setopt(handle, CURLOPT_TIMECONDITION, CURL_TIMECOND_IFMODSINCE);
}

static void mcf_easy_config_time_out(CURL *handle, unsigned int time_out) {
    long t_o = (long) (time_out > 0 ? time_out : 10L);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, t_o);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, t_o);
}

static void mcf_easy_config_signal(CURL *handle, bool is_on) {
    long value = is_on ? 1L : 0L;
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, value);
}

static void mcf_easy_config_proxy(CURL *handle) {
    if (global_proxy) {
        if (strlen(global_proxy->url) > 0 && global_proxy->port > 0) {
            curl_easy_setopt(handle, CURLOPT_PROXY, global_proxy->url);
            curl_easy_setopt(handle, CURLOPT_PROXYPORT, global_proxy->port);
        }
    }
}

static void mcf_easy_config_write_call_back(CURL *handle, mcf_easy_request_response_data *response_data) {

}

static void mcf_easy_config_debug(CURL *handle) {
    curl_easy_setopt(handle, CURLOPT_DEBUGFUNCTION, &debug_func);
    curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
}

static void mcf_easy_log(const char *format, ...) {
    if (mcf_easy_log_callback) {
        static char buffer[LOG_MAX_BUF_SIZE];
        va_list args;
        va_start (args, format);
        vsnprintf (buffer, LOG_MAX_BUF_SIZE, format, args);
        va_end (args);
        mcf_easy_log_callback(buffer);
    }
}