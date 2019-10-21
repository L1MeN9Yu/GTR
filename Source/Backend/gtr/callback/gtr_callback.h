//
//  gtr_callback.h
//  GTR
//
//  Created by Mengyu Li on 2019/10/18.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#ifndef gtr_callback_h
#define gtr_callback_h


typedef void (*on_data_task_succeed)(
        unsigned int task_id, long http_response_code, const void *header_data,
        unsigned long header_data_size, const void *body_data, unsigned long body_data_size
) __attribute__((nonnull(3, 5)));

typedef void (*on_data_task_failed)(
        unsigned int task_id, long http_response_code, int error_code, const char *error_message
) __attribute__((nonnull(4)));

#endif /* gtr_callback_h */
