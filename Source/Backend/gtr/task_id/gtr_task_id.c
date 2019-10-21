//
//  gtr_types.c
//  GTR
//
//  Created by Mengyu Li on 2019/10/15.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#include "gtr_task_id.h"
#include "gtr_atomic.h"

/**
 * 全局唯一当前请求id
 */
static volatile unsigned int gtr_core_request_global_task_id = 0;

unsigned int gtr_task_id_init(void) {
    unsigned int task_id = gtr_atomic_unsigned_int_add_and_fetch(&gtr_core_request_global_task_id, 1);
    return task_id;
}
