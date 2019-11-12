//
//  gtr_core_data_task.h
//  GTR
//
//  Created L1MeN9Yu on 2018/9/2.
//  Copyright © 2018 limengyu.top. All rights reserved.
//

#ifndef gtr_core_request_h
#define gtr_core_request_h

#include <stdio.h>
#include <stdbool.h>
#include <curl/curl.h>
#include "gtr_callback.h"
#include "Bridge.Task.h"

/**
 *
 */
typedef struct gtr_core_race_response_header {
    char *data;
    unsigned long size;
} gtr_core_race_response_header;

/**
 *
 */
typedef struct gtr_core_race_request_body {
    char *data;
    unsigned long size;
    unsigned long size_left;
} gtr_task_request_body;

/**
 *
 */
typedef struct gtr_core_race_response_body {
    char *data;
    unsigned long size;
} gtr_core_race_response_body;

/**
 *
 */
typedef struct gtr_core_race_download_data {
    char *file_path;
    unsigned int task_id;

    void (*on_progress)(unsigned int task_id, unsigned long long now, unsigned long long total);
} gtr_task_download_data;

typedef struct gtr_task_options {
    bool is_debug;
    unsigned int time_out;
    long max_redirects;
} gtr_task_options;

typedef struct gtr_task_speed {
    long max_receive_speed;
    long max_send_speed;
    long low_speed_limit;
    long low_speed_time;
} gtr_task_speed;

typedef struct gtr_task_proxy {
    char *url;
    long port;
} gtr_task_proxy;

/**
 * 请求
 */
struct gtr_core_data_task {
    unsigned int task_id;
    bool is_cancel;
    char *method;
    char *url;
    char *header;
    gtr_task_options options;
    gtr_task_speed speed;
    gtr_task_proxy proxy;
    gtr_task_request_body *request_data;

    /**
     * succeed callback
     * @param task_id
     * @param http_response_code
     * @param header_data
     * @param header_data_size
     * @param body_data
     * @param body_data_size
     */
    on_data_task_succeed on_succeed;
    /**
     *
     * @param task_id
     * @param http_response_code
     * @param error_code
     * @param error_message
     */
    on_data_task_failed on_failed;
};

gtr_core_data_task *gtr_core_data_task_create(unsigned int *task_id, const char *url, const char *header);

void gtr_core_data_task_config_parameters(gtr_core_data_task *core_race, const char *method, const void *param_data, unsigned long param_size);

void gtr_core_data_task_config_options(gtr_core_data_task *core_race, bool is_debug, unsigned int time_out, long max_redirects);

void gtr_core_data_task_config_speed(gtr_core_data_task *core_race, long max_receive_speed, long max_send_speed, long low_speed_limit, long low_speed_time);

void gtr_core_data_task_config_proxy(gtr_core_data_task *core_race, const char *url, long port);

void gtr_core_data_task_config_callback(gtr_core_data_task *core_race, on_data_task_succeed succeed_callback, on_data_task_failed failure_callback);

#endif /* gtr_core_request_h */
