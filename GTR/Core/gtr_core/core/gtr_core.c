//
//  gtr_core.c
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
#include "gtr_core.h"
#include "gtr_core_request.h"
#include "gtr_thread_pool.h"
#include "gtr_atomic.h"
#include "gtr_core_header_helper.h"
#include "gtr_core_proxy.h"
#include "gtr_core_log.h"

/**
 * 请求的线程池
 */
static thread_pool gtr_core_thread_pool;

/**
 * 全局唯一当前请求id
 */
static volatile unsigned int gtr_core_request_global_task_id = 0;

static const char *global_user_agent;

static gtr_core_proxy *global_proxy;

static void
gtr_core_config_http_method(
        CURL *handle,
        gtr_core_request *request
);

static void
gtr_core_config_url(
        CURL *handle,
        const char *url
);

static void
gtr_core_config_accept_encoding(
        CURL *handle
);

static void
gtr_core_config_keep_alive(
        CURL *handle
);

static void
gtr_core_config_headers(
        CURL *handle,
        const char *user_agent, struct curl_slist *header
);

static void
gtr_core_config_verify_peer(
        CURL *handle,
        bool on
);

static void
gtr_core_config_signal(
        CURL *handle,
        bool is_on
);

static void
gtr_core_config_time_out(
        CURL *handle,
        unsigned int time_out
);

static void
gtr_core_config_write_call_back(
        CURL *handle,
        gtr_core_request_response_data *response_data
);

static void
gtr_core_config_progress(
        CURL *handle,
        gtr_core_request *request
);

static void
gtr_core_config_proxy(
        CURL *handle
);

static void
gtr_core_config_debug(
        CURL *handle
);

//--- Call Back
static int
debug_func(
        CURL *__unused handle,
        curl_infotype type,
        char *data,
        size_t size,
        void *__unused user_p
) {
    switch (type) {
        case CURLINFO_TEXT:
            gtr_core_log(gtr_log_flag_trace, "Info: %.*s", size, data);
            break;
        case CURLINFO_HEADER_IN:
            gtr_core_log(gtr_log_flag_trace, "Rx header: %.*s", size, data);
            break;
        case CURLINFO_HEADER_OUT:
            gtr_core_log(gtr_log_flag_trace, "Tx header: %.*s", size, data);
            break;
        case CURLINFO_DATA_IN:
            gtr_core_log(gtr_log_flag_trace, "Rx data: %.*s", size, data);
            break;
        case CURLINFO_DATA_OUT:
            gtr_core_log(gtr_log_flag_trace, "Tx data: %.*s", size, data);
            break;
        case CURLINFO_SSL_DATA_IN:
            gtr_core_log(gtr_log_flag_trace, "Rx SSL data: %.*s", size, data);
            break;
        case CURLINFO_SSL_DATA_OUT:
            gtr_core_log(gtr_log_flag_trace, "Tx SSL data: %.*s", size, data);
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
        void *userp
) {
    gtr_core_request_request_data *request_data = (gtr_core_request_request_data *) userp;
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

/**
 * 写入回调
 * @param contents 内容
 * @param size 内容大小
 * @param nmemb nmemb
 * @param userp userp
 * @return size_t
 */
static size_t
write_callback(
        void *contents,
        size_t size,
        size_t nmemb,
        void *userp
) {
    size_t real_size = size * nmemb;
    gtr_core_request_response_data *response_data = (gtr_core_request_response_data *) userp;

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

static int
progress_callback(
        void *p,
        curl_off_t download_total,
        curl_off_t download_now,
        curl_off_t upload_total,
        curl_off_t upload_now
) {
    gtr_core_log(
            gtr_log_flag_debug,
            "UP: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
            "  DOWN: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
            "\r\n",
            upload_now, upload_total, download_now, download_total);
    return 0;
}

//--- Core
static void
request(
        gtr_core_request *core_request
) {

    CURL *curl_handle;
    CURLcode res;

    gtr_core_request_response_data response_data;

    response_data.response_data = malloc(1);
    response_data.response_data_size = 0;

    curl_handle = curl_easy_init();

    {
        gtr_core_config_http_method(curl_handle, core_request);
    }

    {
        gtr_core_config_url(curl_handle, core_request->url);
    }

    {
        gtr_core_config_accept_encoding(curl_handle);
    }

    {
        gtr_core_config_keep_alive(curl_handle);
    }

    {
        gtr_core_config_verify_peer(curl_handle, true);
    }

    {
        gtr_core_config_time_out(curl_handle, core_request->time_out);
    }

    {
        //如果不使用threaded resolver或者c-ares,需要no_signal=1,否则在dns解析超时情况下crash
        //详见:https://curl.haxx.se/libcurl/c/CURLOPT_NOSIGNAL.html https://curl.haxx.se/libcurl/c/threadsafe.html
        gtr_core_config_signal(curl_handle, true);
    }

    {
        gtr_core_config_write_call_back(curl_handle, &response_data);
    }

    {
        gtr_core_config_progress(curl_handle, core_request);
    }

    {
        gtr_core_config_proxy(curl_handle);
    }

    {
        gtr_core_config_debug(curl_handle);
    }

    struct curl_slist *header = gtr_core_add_custom_headers(core_request->header);
    gtr_core_config_headers(curl_handle, global_user_agent, header);

    {
        res = curl_easy_perform(curl_handle);
    }

    {
        long http_response_code = 0;
        curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_response_code);

        /* check for errors */
        if (res != CURLE_OK) {
            gtr_core_log(gtr_log_flag_error, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            if (core_request->on_failed) {
                core_request->on_failed(core_request->task_id, http_response_code, res, curl_easy_strerror(res));
            }
        } else {
            gtr_core_log(gtr_log_flag_trace, "%lu bytes retrieved\n", response_data.response_data_size);
            if (core_request->on_succeed) {
                core_request->on_succeed(core_request->task_id, http_response_code, response_data.response_data, response_data.response_data_size);
            }
        }
    }

    /* cleanup curl stuff */
    if (header) {
        curl_slist_free_all(header);
    }

    {
        curl_easy_cleanup(curl_handle);
        free(core_request->url);
        free(core_request);
        free(response_data.response_data);
    }
}

//--- Public
void
gtr_core_init(
        const char *user_agent,
        void *log_callback
) {
    if (user_agent) {
        size_t user_agent_size = strlen(user_agent) + 1;
        global_user_agent = malloc(user_agent_size);
        memcpy((void *) global_user_agent, user_agent, user_agent_size);
    } else {
        size_t user_agent_size = strlen("top.limengyu.GTR") + 1;
        global_user_agent = malloc(user_agent_size);
        memcpy((void *) global_user_agent, "top.limengyu.GTR", user_agent_size);
    }
    config_log_callback(log_callback);
    curl_global_init(CURL_GLOBAL_ALL);
    gtr_core_thread_pool = thread_pool_init(10);
    thread_pool_wait(gtr_core_thread_pool);
    gtr_core_log(gtr_log_flag_info, "gtr : global_user_agent = %s", global_user_agent);
    gtr_core_log(gtr_log_flag_info, "curl version : %s", curl_version());
}

void __unused
gtr_core_dispose(
        void
) {
    curl_global_cleanup();
    thread_pool_destroy(gtr_core_thread_pool);
}

void
gtr_core_open_proxy(
        const char *url,
        unsigned int port
) {
    assert(url);
    assert(port > 0);
    global_proxy = (gtr_core_proxy *) calloc(1, sizeof(gtr_core_proxy));
    size_t url_size = strlen(url) + 1;
    global_proxy->url = malloc(url_size);
    memcpy(global_proxy->url, url, url_size);
    global_proxy->port = port;
}

void
gtr_core_close_proxy(
        void
) {
    if (global_proxy) {
        if (global_proxy->url) {
            free(global_proxy->url);
        }
        global_proxy = NULL;
    }
}

static void
gtr_core_go_request(
        unsigned int *task_id,
        gtr_core_request_type type,
        const char *url,
        const char *header,
        unsigned int time_out,
        const void *request_data,
        unsigned long request_data_size,
        void *succeed_callback,
        void *failure_callback,
        const char *file_path,
        void *progress_callback
) {
    gtr_core_request *core_request = (gtr_core_request *) calloc(1, sizeof(gtr_core_request));

    {
        *task_id = gtr_atomic_unsigned_int_add_and_fetch(&gtr_core_request_global_task_id, 1);
        core_request->task_id = *task_id;
    }

    {
        core_request->is_cancel = false;
        core_request->request_type = type;
    }

    {
        //url
        assert(url);
        if (url) {
            size_t url_size = strlen(url) + 1;
            core_request->url = malloc(url_size);
            memcpy(core_request->url, url, url_size);
        }
    }
    {
        //header
        if (header) {
            size_t header_size = strlen(header) + 1;
            core_request->header = malloc(header_size);
            memcpy(core_request->header, header, header_size);
        }
    }

    {
        //data
        if (request_data_size > 0 && request_data != NULL) {
            core_request->request_data = (gtr_core_request_request_data *) calloc(1, sizeof(gtr_core_request_request_data));
            core_request->request_data->data = malloc((size_t) request_data_size);
            core_request->request_data->size_left = request_data_size;
            core_request->request_data->size = request_data_size;
            if (core_request->request_data->data == NULL) {
                //error
            } else {
                memcpy(core_request->request_data->data, request_data, request_data_size);
            }
        } else {
            core_request->request_data = NULL;
        }
    }
    {
        //time out
        core_request->time_out = time_out;
    }
    {
        //call_back
        core_request->on_succeed = succeed_callback;
        core_request->on_failed = failure_callback;
    }

    {
        if (file_path) {
            core_request->download_data = calloc(1, sizeof(gtr_core_request_download_data));
            size_t file_path_size = strlen(file_path) + 1;
            core_request->download_data->file_path = malloc(file_path_size);
            memcpy(core_request->download_data->file_path, file_path, file_path_size);
            core_request->download_data->on_progress = progress_callback;
            core_request->download_data->task_id = *task_id;
        }
    }

    thread_pool_add_work(gtr_core_thread_pool, (void *) request, core_request);
}

void
gtr_core_add_request(
        unsigned int *task_id,
        gtr_core_request_type type,
        const char *url,
        const char *header,
        unsigned int time_out,
        const void *request_data,
        unsigned long request_data_size,
        void *succeed_callback,
        void *failure_callback
) {
    gtr_core_go_request(
            task_id,
            type,
            url,
            header,
            time_out,
            request_data,
            request_data_size,
            succeed_callback,
            failure_callback,
            NULL,
            NULL);
}

void gtr_core_add_download_request(
        unsigned int *task_id,
        const char *url,
        const char *file_path,
        const char *header,
        unsigned int time_out,
        void *progress_callback,
        void *succeed_callback,
        void *failure_callback
) {
    gtr_core_go_request(
            task_id,
            gtr_core_request_type_download,
            url,
            header,
            time_out,
            NULL,
            0,
            succeed_callback,
            failure_callback,
            file_path,
            progress_callback);
}

//---Private Config
static void
gtr_core_config_http_method(
        CURL *handle,
        gtr_core_request *request
) {
    if (!request) {
        assert(request != NULL);
        return;
    }
    switch (request->request_type) {
        case gtr_core_request_type_get:
            curl_easy_setopt(handle, CURLOPT_HTTPGET, 1L);
            break;
        case gtr_core_request_type_post:
            curl_easy_setopt(handle, CURLOPT_POST, 1L);
            curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_callback);
            curl_easy_setopt(handle, CURLOPT_READDATA, request->request_data);
            curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, request->request_data->size);
            curl_easy_setopt(handle, CURLOPT_INFILESIZE, request->request_data->size);
            break;
        case gtr_core_request_type_put:
            curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_callback);
            curl_easy_setopt(handle, CURLOPT_READDATA, request->request_data->data);
            curl_easy_setopt(handle, CURLOPT_INFILESIZE, request->request_data->size);
            break;
        case gtr_core_request_type_download:
            curl_easy_setopt(handle, CURLOPT_HTTPGET, 1L);
            break;
        case gtr_core_request_type_upload:
            curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
            break;
    }
}

static void
gtr_core_config_url(
        CURL *handle,
        const char *url
) {
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
}

static void
gtr_core_config_accept_encoding(
        CURL *handle
) {
    //设置CURLOPT_ACCEPT_ENCODING
    curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "");
}

static void
gtr_core_config_keep_alive(
        CURL *handle
) {
    /* enable TCP keep-alive for this transfer */
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);
    /* keep-alive idle time to 120 seconds */
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPIDLE, 120L);
    /* interval time between keep-alive probes: 60 seconds */
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPINTVL, 60L);
}

static void
gtr_core_config_headers(
        CURL *handle,
        const char *user_agent,
        struct curl_slist *header
) {
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

static void
gtr_core_config_verify_peer(
        CURL *handle,
        bool on
) {
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, on ? 1L : 0L);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, on ? 1L : 0L);
}

//TODO ETag/If_Modified_since
static void
gtr_core_config_time_condition(
        CURL *handle,
        unsigned long time
) {
    /* January 1, 2020 is 1577833200 */
    curl_easy_setopt(handle, CURLOPT_TIMEVALUE, time);

    /* If-Modified-Since the above time stamp */
    curl_easy_setopt(handle, CURLOPT_TIMECONDITION, CURL_TIMECOND_IFMODSINCE);
}

static void
gtr_core_config_progress(
        CURL *handle,
        gtr_core_request *request
) {
    assert(request);
    //上传和下载才需要进度回掉方法
    switch (request->request_type) {
        case gtr_core_request_type_get:
            curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
            break;
        case gtr_core_request_type_post:
            curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
            break;
        case gtr_core_request_type_put:
            curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
            break;
        case gtr_core_request_type_download:
            curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(handle, CURLOPT_XFERINFOFUNCTION, progress_callback);
            curl_easy_setopt(handle, CURLOPT_XFERINFODATA, &request->download_data);
            break;
        case gtr_core_request_type_upload:
            curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(handle, CURLOPT_XFERINFOFUNCTION, progress_callback);
            curl_easy_setopt(handle, CURLOPT_XFERINFODATA, NULL);
            break;
    }
}

static void
gtr_core_config_time_out(
        CURL *handle,
        unsigned int time_out
) {
    long t_o = (long) (time_out > 0 ? time_out : 10L);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, t_o);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, t_o);
}

static void
gtr_core_config_signal(
        CURL *handle,
        bool is_on
) {
    long value = is_on ? 1L : 0L;
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, value);
}

static void
gtr_core_config_proxy(
        CURL *handle
) {
    if (global_proxy) {
        if (strlen(global_proxy->url) > 0 && global_proxy->port > 0) {
            curl_easy_setopt(handle, CURLOPT_PROXY, global_proxy->url);
            curl_easy_setopt(handle, CURLOPT_PROXYPORT, global_proxy->port);
        }
    }
}

static void
gtr_core_config_write_call_back(
        CURL *handle,
        gtr_core_request_response_data *response_data
) {
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *) response_data);
}

static void
gtr_core_config_debug(
        CURL *handle
) {
    curl_easy_setopt(handle, CURLOPT_DEBUGFUNCTION, &debug_func);
    curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
}
