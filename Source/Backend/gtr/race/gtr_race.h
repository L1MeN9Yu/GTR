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
typedef struct gtr_task_response_header {
    char *data;
    unsigned long size;
} gtr_task_response_header;

/**
 *
 */
typedef struct gtr_data_task_request_body {
    char *data;
    unsigned long size;
    unsigned long size_left;
} gtr_data_task_request_body;

/**
 *
 */
typedef struct gtr_data_task_response_body {
    char *data;
    unsigned long size;
} gtr_data_task_response_body;

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

typedef struct gtr_task_response_info_options {
    bool base;
    bool time;
    bool size;
    bool speed;
    bool ssl;
    bool socket;
    bool cookie;
} gtr_task_response_info_options;

/**
 * 请求
 */
struct gtr_core_data_task {
    CURL *curl;
    unsigned int task_id;
    bool is_cancel;
    char *method;
    char *url;
    char *header;
    curl_mime *mime;

    gtr_task_options options;
    gtr_task_speed speed;
    gtr_task_proxy *proxy;
    /**
     * response info options
     */
    gtr_task_response_info_options response_info_options;
    /**
     * request body
     */
    gtr_data_task_request_body *request_data;

    /**
     * succeed callback
     */
    on_data_task_succeed on_succeed;

    /**
     * failed callback
     */
    on_data_task_failed on_failed;
};

struct gtr_core_upload_task {
    unsigned int task_id;
    char *url;
    char *header;
    gtr_task_options options;
    gtr_task_speed speed;
    gtr_task_proxy *proxy;
    gtr_task_response_info_options response_info_options;
    on_task_progress on_progress;
    on_upload_task_succeed on_succeed;
    on_upload_task_failed on_failed;
};

//-------------------------------------//
gtr_core_data_task *gtr_core_data_task_create(unsigned int *task_id, const char *url, const char *header);

void gtr_core_data_task_config_parameters(gtr_core_data_task *data_task, const char *method, const void *param_data, unsigned long param_size);

void gtr_core_data_task_add_form_data(gtr_core_data_task *data_task, int type, const char *name, const char *value);

void gtr_core_data_task_config_options(gtr_core_data_task *data_task, bool is_debug, unsigned int time_out, long max_redirects);

void gtr_core_data_task_config_response_info_options(gtr_core_data_task *data_task, bool base, bool time, bool size, bool speed, bool ssl, bool socket, bool cookie);

void gtr_core_data_task_config_speed(gtr_core_data_task *data_task, long max_receive_speed, long max_send_speed, long low_speed_limit, long low_speed_time);

void gtr_core_data_task_config_proxy(gtr_core_data_task *data_task, const char *url, long port);

void gtr_core_data_task_config_callback(gtr_core_data_task *data_task, on_data_task_succeed succeed_callback, on_data_task_failed failure_callback);
//-------------------------------------//

#endif /* gtr_core_request_h */
