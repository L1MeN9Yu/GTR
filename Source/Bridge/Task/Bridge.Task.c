#include <stdio.h>
#include <stdbool.h>
#include "gtr_core.h"
#include "gtr_get.h"
#include "Bridge.Callback.h"
#include "gtr_post.h"
#include "gtr_custom.h"
#include "Bridge.Task.h"

//---------- Export
extern void swift_get_request_succeed(unsigned int task_id, void *c_header_data, unsigned long c_header_data_size, void *c_body_data, unsigned long c_body_data_size);

extern void swift_get_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

extern void swift_post_request_succeed(unsigned int task_id, void *c_header_data, unsigned long c_header_data_size, void *c_body_data, unsigned long c_body_data_size);

extern void swift_post_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

extern void swift_put_request_succeed(unsigned int task_id, void *c_header_data, unsigned long c_header_data_size, void *c_body_data, unsigned long c_body_data_size);

extern void swift_put_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

extern void swift_download_progress(unsigned int task_id, unsigned long long now, unsigned long long total);

extern void swift_download_request_succeed(unsigned int task_id, void *c_data, unsigned long c_date_size);

extern void swift_download_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

//---------- 前置定义

static void on_http_download_file_progress(unsigned int task_id, unsigned long long downloaded_size, unsigned long long total_size);

static void on_http_download_file_success(unsigned int task_id, long http_response_code, void *data, unsigned long size);

static void on_http_download_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

/// on_data_task_succeed_callback
static void on_data_task_succeed_callback(unsigned int task_id, long http_response_code, const void *header_data, unsigned long header_data_size, const void *body_data, unsigned long body_data_size);

/// on_data_task_failed_callback
static void on_data_task_failed_callback(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

//---------- Get
void gtr_get(unsigned *task_id, const char *url, const char *headers, unsigned int time_out, long speed_limit) {
    gtr_add_get_request(task_id, url, headers, time_out, speed_limit, &on_data_task_succeed_callback, &on_data_task_failed_callback);
}
//----------

//---------- Post
void gtr_post(unsigned int *task_id, const char *url, const char *headers, unsigned int time_out, long speed_limit, const void *param_data, unsigned long param_size) {
    gtr_add_post_request(task_id, url, headers, time_out, speed_limit, param_data, param_size, &on_data_task_succeed_callback, &on_data_task_failed_callback);
}
//----------

//---------- CUSTOM
void gtr_custom(unsigned int *task_id, const char *url, const char *headers, const char *method, unsigned int time_out, long speed_limit, const void *param_data, unsigned long param_size) {
    gtr_add_custom_request(task_id, url, headers, method, time_out, speed_limit, param_data, param_size, &on_data_task_succeed_callback, &on_data_task_failed_callback);
}

gtr_core_race *gtr_data_task_create(unsigned *task_id, const char *url, const char *headers) {
    gtr_core_race *core_race = gtr_core_data_task_create(task_id, url, headers);
    return core_race;
}

void gtr_data_task_config_parameters(gtr_core_race *core_race, const char *method, const void *param_data, unsigned long param_size) {
    gtr_core_data_task_config_parameters(core_race, method, param_data, param_size);
}

void gtr_data_task_config_options(gtr_core_race *core_race, bool is_debug, unsigned int time_out, long speed_limit) {
    gtr_core_data_task_config_options(core_race, is_debug, time_out, speed_limit);
}

void gtr_data_task_start(gtr_core_race *core_race) {
    gtr_core_data_task_config_callback(core_race, &on_data_task_succeed_callback, &on_data_task_failed_callback);
    gtr_core_race_start(core_race);
}

//---------- 下载文件
extern void gtr_download(unsigned int *task_id, const char *url, const char *file_path, const char *headers, unsigned int time_out, long speed_limit) {
    //ToDo [L1MeN9Yu]
}

//---------- 取消请求
extern void gtr_cancel(unsigned int task_id) {

}

static void on_data_task_succeed_callback(unsigned int task_id, long http_response_code, const void *header_data, unsigned long header_data_size, const void *body_data, unsigned long body_data_size) {
    swift_data_task_succeed(task_id, header_data, header_data_size, body_data, body_data_size);
}

static void on_data_task_failed_callback(unsigned int task_id, long http_response_code, int error_code, const char *error_message) {
    swift_data_task_failed(task_id, http_response_code, error_code, error_message);
}

static void on_http_download_file_progress(unsigned int task_id, unsigned long long downloaded_size, unsigned long long total_size) {
    swift_download_progress(task_id, downloaded_size, total_size);
}

static void on_http_download_file_success(unsigned int task_id, long http_response_code, void *data, unsigned long size) {
    swift_download_request_succeed(task_id, data, size);
}

static void on_http_download_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message) {
    swift_download_request_failure(task_id, http_response_code, error_code, error_message);
}
