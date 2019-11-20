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
        unsigned int task_id,
        const void *response_info_data, long response_info_data_size,
        const void *header_data, long header_data_size,
        const void *body_data, long body_data_size
) __attribute__((nonnull(2, 4, 6)));

typedef void (*on_data_task_failed)(
        unsigned int task_id,
        const void *response_info_data, long response_info_data_size,
        int error_code, const char *error_message
) __attribute__((nonnull(2, 5)));

typedef void (*on_task_progress)(
        unsigned int task_id,
        unsigned long long now,
        unsigned long long total
);

typedef on_data_task_failed on_upload_task_failed;
typedef on_data_task_succeed on_upload_task_succeed;

#endif /* gtr_callback_h */
