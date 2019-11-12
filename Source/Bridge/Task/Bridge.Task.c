#include <stdio.h>
#include <stdbool.h>
#include "gtr_core.h"
#include "Bridge.Callback.h"
#include "Bridge.Task.h"

//---------- Export
extern void swift_download_progress(unsigned int task_id, unsigned long long now, unsigned long long total);

extern void swift_download_request_succeed(unsigned int task_id, void *c_data, unsigned long c_date_size);

extern void swift_download_request_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

//---------- 前置定义

static void on_http_download_file_progress(unsigned int task_id, unsigned long long downloaded_size, unsigned long long total_size);

static void on_http_download_file_success(unsigned int task_id, long http_response_code, void *data, unsigned long size);

static void on_http_download_failure(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

/// on_data_task_succeed_callback
static void on_data_task_succeed_callback(unsigned int task_id, const void *response_info_data, long response_info_data_size, const void *header_data, long header_data_size, const void *body_data, long body_data_size);

/// on_data_task_failed_callback
static void on_data_task_failed_callback(unsigned int task_id, const void *response_info_data, long response_info_data_size, int error_code, const char *error_message);

gtr_core_data_task *gtr_data_task_create(unsigned *task_id, const char *url, const char *headers) {
    gtr_core_data_task *core_race = gtr_core_data_task_create(task_id, url, headers);
    return core_race;
}

void gtr_data_task_config_parameters(gtr_core_data_task *core_race, const char *method, const void *param_data, unsigned long param_size) {
    gtr_core_data_task_config_parameters(core_race, method, param_data, param_size);
}

void gtr_data_task_config_options(gtr_core_data_task *core_race, bool is_debug, unsigned int time_out, long max_redirects) {
    gtr_core_data_task_config_options(core_race, is_debug, time_out, max_redirects);
}

void gtr_data_task_config_speed(gtr_core_data_task *core_race, long max_receive_speed, long max_send_speed, long low_speed_limit, long low_speed_time) {
    gtr_core_data_task_config_speed(core_race, max_receive_speed, max_send_speed, low_speed_limit, low_speed_time);
}

void gtr_data_task_config_proxy(gtr_core_data_task *core_race, const char *url, long port) {
    gtr_core_data_task_config_proxy(core_race, url, port);
}

void gtr_data_task_start(gtr_core_data_task *core_race) {
    gtr_core_data_task_config_callback(core_race, &on_data_task_succeed_callback, &on_data_task_failed_callback);
    gtr_core_data_task_start(core_race);
}

//---------- 下载文件
extern void gtr_download(unsigned int *task_id, const char *url, const char *file_path, const char *headers, unsigned int time_out, long speed_limit) {
    //ToDo [L1MeN9Yu]
}

//---------- 取消请求
extern void gtr_cancel(unsigned int task_id) {

}

static void on_data_task_succeed_callback(unsigned int task_id, const void *response_info_data, long response_info_data_size, const void *header_data, long header_data_size, const void *body_data, long body_data_size) {
    swift_data_task_succeed(task_id, response_info_data, response_info_data_size, header_data, header_data_size, body_data, body_data_size);
}

static void on_data_task_failed_callback(unsigned int task_id, const void *response_info_data, long response_info_data_size, int error_code, const char *error_message) {
    swift_data_task_failed(task_id, response_info_data, response_info_data_size, error_code, error_message);
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
