//
//  Bridge.Callback.h
//  GTR
//
//  Created by Mengyu Li on 2019/10/18.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#ifndef Bridge_Callback_h
#define Bridge_Callback_h

__attribute__((nonnull(4, 6)))
extern void swift_data_task_succeed(unsigned int task_id, const void *response_info_data, long response_info_data_size, const void *c_header_data, long c_header_data_size, const void *c_body_data, long c_body_data_size);

__attribute__((nonnull(5)))
extern void swift_data_task_failed(unsigned int task_id, const void *response_info_data, long response_info_data_size, int error_code, const char *error_message);

#endif /* Bridge_Callback_h */
