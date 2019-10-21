//
//  gtr_custom.h
//  GTR
//
//  Created by Mengyu Li on 2019/10/18.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#ifndef gtr_custom_h
#define gtr_custom_h

#include "gtr_callback.h"

__attribute__((nonnull(1, 2)))
void gtr_add_custom_request(
        unsigned int *task_id, const char *url, const char *headers, const char *method, unsigned int time_out, long speed_limit,
        const void *param_data, unsigned long param_size,
        on_data_task_succeed succeed,
        on_data_task_failed failed
);


#endif /* gtr_custom_h */
