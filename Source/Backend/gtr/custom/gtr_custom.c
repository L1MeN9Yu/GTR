//
//  gtr_custom.c
//  GTR
//
//  Created by Mengyu Li on 2019/10/18.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#include "gtr_custom.h"
#include "gtr_core.h"

void gtr_add_custom_request(
        unsigned int *task_id, const char *url, const char *headers, const char *method, unsigned int time_out, long speed_limit,
        const void *param_data, unsigned long param_size,
        on_data_task_succeed succeed, on_data_task_failed failed
) {
    gtr_core_race *data_task = gtr_data_task_init(task_id, method, url, headers, time_out, speed_limit, param_data, param_size, succeed, failed);
    gtr_core_race_start(data_task);
}
