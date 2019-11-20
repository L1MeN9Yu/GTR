//
//  gtr_core_data_task.c
//  GTR
//
//  Created L1MeN9Yu on 2018/9/2.
//  Copyright © 2018 limengyu.top. All rights reserved.
//

#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include "gtr_race.h"
#include "gtr_task_id.h"

gtr_core_data_task *gtr_core_data_task_create(unsigned int *task_id, const char *url, const char *header) {
    assert(url);

    gtr_core_data_task *data_task = (gtr_core_data_task *) malloc(sizeof(gtr_core_data_task));

    *task_id = gtr_task_id_init();
    data_task->task_id = *task_id;

    data_task->is_cancel = false;

    data_task->url = strdup(url);

    data_task->proxy = NULL;

    data_task->mime = NULL;

    if (header) {data_task->header = strdup(header);}

    data_task->curl = curl_easy_init();

    return data_task;
}

void gtr_core_data_task_config_parameters(gtr_core_data_task *data_task, const char *method, const void *param_data, unsigned long param_size) {
    data_task->method = strdup(method);

    if (param_size > 0 && param_data != NULL) {
        data_task->request_data = (gtr_data_task_request_body *) calloc(1, sizeof(gtr_data_task_request_body));
        data_task->request_data->data = malloc((size_t) param_data);
        data_task->request_data->size_left = param_size;
        data_task->request_data->size = param_size;
        memcpy(data_task->request_data->data, param_data, param_size);
    } else {
        data_task->request_data = NULL;
    }
}

void gtr_core_data_task_add_form_data(gtr_core_data_task *data_task, int type, const char *name, const char *value) {
    if (data_task->mime == NULL) {
        data_task->mime = curl_mime_init(data_task->curl);
    }
    switch (type) {
        case 0: {
            curl_mimepart *part = curl_mime_addpart(data_task->mime);
            curl_mime_data(part, value, CURL_ZERO_TERMINATED);
            curl_mime_name(part, name);
        }
            break;
        case 1: {
            curl_mimepart *part = curl_mime_addpart(data_task->mime);
            curl_mime_filedata(part, value);
            curl_mime_name(part, name);
        }
            break;
        default:
            break;
    }
}

void gtr_core_data_task_config_options(gtr_core_data_task *data_task, bool is_debug, unsigned int time_out, long max_redirects) {
    gtr_task_options options = {is_debug, time_out, max_redirects};
    data_task->options = options;
}

void gtr_core_data_task_config_response_info_options(gtr_core_data_task *data_task, bool base, bool time, bool size, bool speed, bool ssl, bool socket, bool cookie) {
    gtr_task_response_info_options options = {base, time, size, speed, ssl, socket, cookie};
    data_task->response_info_options = options;
}

void gtr_core_data_task_config_speed(gtr_core_data_task *data_task, long max_receive_speed, long max_send_speed, long low_speed_limit, long low_speed_time) {
    gtr_task_speed speed = {max_receive_speed, max_send_speed, low_speed_limit, low_speed_time};
    data_task->speed = speed;
}

void gtr_core_data_task_config_proxy(gtr_core_data_task *data_task, const char *url, long port) {
    if (url && strlen(url) > 0 && port > 0) {
        data_task->proxy = malloc(sizeof(gtr_task_proxy));
        data_task->proxy->url = strdup(url);
        data_task->proxy->port = port;
    }
}

void gtr_core_data_task_config_callback(gtr_core_data_task *data_task, on_data_task_succeed succeed_callback, on_data_task_failed failure_callback) {
    data_task->on_succeed = succeed_callback;
    data_task->on_failed = failure_callback;
}