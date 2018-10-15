#include <stdio.h>
#include <stdbool.h>
#include "mcf_easy.h"

//---------- Export
extern void swift_get_request_succeed(unsigned int task_id, void *c_data, unsigned long c_date_size);

extern void swift_get_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

extern void swift_post_request_succeed(unsigned int task_id, void *c_data, unsigned long c_date_size);

extern void swift_post_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

extern void swift_put_request_succeed(unsigned int task_id, void *c_data, unsigned long c_date_size);

extern void swift_put_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

extern void swift_log_callback(char *message);


//---------- 前置定义
static void mcf_curl_log_message_call_back(char *message);

static void on_http_get_request_succeed(unsigned int task_id, long http_response_code, void *data, unsigned long data_size);

static void on_http_get_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

static void on_http_post_request_succeed(unsigned int task_id, long http_response_code, void *data, unsigned long size);

static void on_http_post_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

static void on_http_put_request_completed(unsigned int task_id, long http_response_code, void *data, unsigned long size);

static void on_http_put_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message);
//----------

//---------- 初始化
void gtr_init(const char *user_agent) {
    mcf_easy_init(user_agent, &mcf_curl_log_message_call_back);
}
//----------

//---------- 设置代理
void gtr_proxy(bool enable, const char *url, unsigned int port) {
    if (enable) {
        mcf_easy_open_proxy(url, port);
    } else {
        mcf_easy_close_proxy();
    }
}

//---------- Get请求
void gtr_get(
        unsigned *task_id,
        const char *url,
        const char *headers,
        unsigned int time_out
) {
    mcf_easy_add_request(
            task_id,
            mcf_easy_request_type_get,
            url,
            headers,
            time_out,
            NULL,
            0,
            &on_http_get_request_succeed,
            &on_http_get_request_failure);
}
//----------

//---------- Post请求
void gtr_post(
        unsigned int *task_id,
        const char *url,
        const char *headers,
        unsigned int time_out,
        const void *param_data,
        unsigned long param_size
) {
    mcf_easy_add_request(
            task_id,
            mcf_easy_request_type_post,
            url,
            headers,
            time_out,
            param_data,
            param_size,
            &on_http_post_request_succeed,
            &on_http_post_request_failure);
}
//----------

//---------- PUT请求
void gtr_put(
        unsigned int *task_id,
        const char *url,
        const char *headers,
        unsigned int time_out,
        const void *param_data,
        unsigned long param_size
) {
    mcf_easy_add_request(
            task_id,
            mcf_easy_request_type_put,
            url,
            headers,
            time_out,
            param_data,
            param_size,
            &on_http_put_request_completed,
            &on_http_put_request_failure
    );
}
//----------

static void mcf_curl_log_message_call_back(char *message) {
    swift_log_callback(message);
}

static void on_http_get_request_succeed(unsigned int task_id, long http_response_code, void *data, unsigned long data_size) {
    swift_get_request_succeed(task_id, data, data_size);
}

static void on_http_get_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message) {
    swift_get_request_failure(task_id, http_response_code, error_code, error_message);
}

static void on_http_put_request_completed(unsigned int task_id, long http_response_code, void *data, unsigned long size) {
    swift_put_request_succeed(task_id, data, size);
}

static void on_http_put_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message) {
    swift_put_request_failure(task_id, http_response_code, error_code, error_message);
}

static void on_http_post_request_succeed(unsigned int task_id, long http_response_code, void *data, unsigned long size) {
    swift_post_request_succeed(task_id, data, size);
}

static void on_http_post_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message) {
    swift_post_request_failure(task_id, http_response_code, error_code, error_message);
}

static void on_http_download_file_progress(unsigned int task_id, size_t downloaded_size) {

}

static void on_http_download_file_success(unsigned int task_id, long http_response_code, void *data) {

}

static void on_http_download_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message) {

}
