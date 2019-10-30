//
//  gtr_core_race.h
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
} gtr_core_race_request_body;

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
} gtr_core_race_download_data;

/**
 * 请求
 */
struct gtr_core_race {
    unsigned int task_id;
    bool is_cancel;
    char *method;
    char *url;
    char *header;
    bool is_debug;
    unsigned int time_out;
    long speed_limit;
    gtr_core_race_request_body *request_data;
    gtr_core_race_download_data *download_data;

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

gtr_core_race *gtr_data_task_init(
        unsigned int *task_id,
        const char *method,
        const char *url,
        const char *header,
        unsigned int time_out,
        long speed_limit,
        const void *request_data,
        unsigned long request_data_size,
        on_data_task_succeed succeed_callback,
        on_data_task_failed failure_callback
);

gtr_core_race *gtr_core_data_task_create(unsigned int *task_id, const char *url, const char *header);

void gtr_core_data_task_config_parameters(gtr_core_race *core_race, const char *method, const void *param_data, unsigned long param_size);

void gtr_core_data_task_config_options(gtr_core_race *core_race, bool is_debug, unsigned int time_out, long speed_limit);

void gtr_core_data_task_config_callback(gtr_core_race *core_race, on_data_task_succeed succeed_callback, on_data_task_failed failure_callback);

#endif /* gtr_core_request_h */
