//
//  gtr_get.h
//  GTR
//
//  Created by Mengyu Li on 2019/10/15.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#ifndef gtr_get_h
#define gtr_get_h

#include <stdio.h>
#include "gtr_callback.h"

__attribute__((nonnull(1, 2)))
void gtr_add_get_request(
        unsigned int *task_id, const char *url, const char *headers, unsigned int time_out, long speed_limit,
        on_data_task_succeed succeed,
        on_data_task_failed failed
);

#endif /* gtr_get_h */
