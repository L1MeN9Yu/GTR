//
//  gtr_get.c
//  GTR
//
//  Created by Mengyu Li on 2019/10/15.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#include "gtr_get.h"
#include "gtr_race.h"
#include "gtr_method.h"
#include "gtr_core.h"

void gtr_add_get_request(
        unsigned int *task_id, const char *url, const char *headers, unsigned int time_out, long speed_limit,
        on_data_task_succeed succeed,
        on_data_task_failed failed) {
    gtr_core_race *data_task = gtr_data_task_init(task_id, METHOD_GET, url, headers, time_out, speed_limit, NULL, 0, succeed, failed);
    gtr_core_race_start(data_task);
}
