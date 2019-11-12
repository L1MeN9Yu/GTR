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

    gtr_core_data_task *core_request = (gtr_core_data_task *) malloc(sizeof(gtr_core_data_task));

    *task_id = gtr_task_id_init();
    core_request->task_id = *task_id;

    core_request->is_cancel = false;

    core_request->url = strdup(url);

    core_request->proxy = NULL;

    if (header) {core_request->header = strdup(header);}
    return core_request;
}

void gtr_core_data_task_config_parameters(gtr_core_data_task *core_race, const char *method, const void *param_data, unsigned long param_size) {
    core_race->method = strdup(method);

    if (param_size > 0 && param_data != NULL) {
        core_race->request_data = (gtr_task_request_body *) calloc(1, sizeof(gtr_task_request_body));
        core_race->request_data->data = malloc((size_t) param_data);
        core_race->request_data->size_left = param_size;
        core_race->request_data->size = param_size;
        memcpy(core_race->request_data->data, param_data, param_size);
    } else {
        core_race->request_data = NULL;
    }
}

void gtr_core_data_task_config_options(gtr_core_data_task *core_race, bool is_debug, unsigned int time_out, long max_redirects) {
    gtr_task_options options = {is_debug, time_out, max_redirects};
    core_race->options = options;
}

void gtr_core_data_task_config_speed(gtr_core_data_task *core_race, long max_receive_speed, long max_send_speed, long low_speed_limit, long low_speed_time) {
    gtr_task_speed speed = {max_receive_speed, max_send_speed, low_speed_limit, low_speed_time};
    core_race->speed = speed;
}

void gtr_core_data_task_config_proxy(gtr_core_data_task *core_race, const char *url, long port) {
    if (url && strlen(url) > 0 && port > 0) {
        core_race->proxy = malloc(sizeof(gtr_task_proxy));
        core_race->proxy->url = strdup(url);
        core_race->proxy->port = port;
    }
}

void gtr_core_data_task_config_callback(gtr_core_data_task *core_race, on_data_task_succeed succeed_callback, on_data_task_failed failure_callback) {
    core_race->on_succeed = succeed_callback;
    core_race->on_failed = failure_callback;
}
