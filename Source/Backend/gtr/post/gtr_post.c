//
//  gtr_post.c
//  GTR
//
//  Created by Mengyu Li on 2019/10/18.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#include "gtr_post.h"
#include "gtr_core.h"
#include "gtr_method.h"

void gtr_add_post_request(
        unsigned int *task_id, const char *url, const char *headers, unsigned int time_out, long speed_limit,
        const void *param_data, unsigned long param_size,
        on_data_task_succeed succeed,
        on_data_task_failed failed) {
    gtr_core_race *data_task = gtr_data_task_init(task_id, METHOD_POST, url, headers, time_out, speed_limit, param_data, param_size, succeed, failed);
    gtr_core_race_start(data_task);
}
