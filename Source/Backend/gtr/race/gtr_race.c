//
//  gtr_core_race.c
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
) {
    assert(url);

    gtr_core_race *core_request = (gtr_core_race *) malloc(sizeof(gtr_core_race));

    *task_id = gtr_task_id_init();
    core_request->task_id = *task_id;

    core_request->method = strdup(method);

    core_request->is_cancel = false;

    core_request->url = strdup(url);

    if (header) {core_request->header = strdup(header);}

    //data
    if (request_data_size > 0 && request_data != NULL) {
        core_request->request_data = (gtr_core_race_request_body *) calloc(1, sizeof(gtr_core_race_request_body));
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

    core_request->time_out = time_out;
    core_request->speed_limit = speed_limit;
    core_request->on_succeed = succeed_callback;
    core_request->on_failed = failure_callback;

    return core_request;
}
