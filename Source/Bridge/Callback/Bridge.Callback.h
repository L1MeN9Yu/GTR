//
//  Bridge.Callback.h
//  GTR
//
//  Created by Mengyu Li on 2019/10/18.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#ifndef Bridge_Callback_h
#define Bridge_Callback_h

__attribute__((nonnull(2, 4)))
extern void swift_data_task_succeed(unsigned int task_id, const void *c_header_data, unsigned long c_header_data_size, const void *c_body_data, unsigned long c_body_data_size);

__attribute__((nonnull(4)))
extern void swift_data_task_failed(unsigned int task_id, long http_response_code, int error_code, const char *error_message);

#endif /* Bridge_Callback_h */
