//
//  GTR.External.h
//  GTR
//
//  Created L1MeN9Yu on 2018/8/29.
//  Copyright © 2018 limengyu.top. All rights reserved.
//

#ifndef GTR_External_h
#define GTR_External_h

extern void gtr_init(const char *user_agent);

extern void gtr_proxy(bool enable, const char *url, unsigned int port);

extern void gtr_get(unsigned int *task_id, const char *url, const char *headers, unsigned int time_out);

extern void gtr_post(unsigned int *task_id, const char *url, const char *headers, unsigned int time_out, const void *param_data, unsigned long param_size);

extern void gtr_put(unsigned int *task_id, const char *url, const char *headers, unsigned int time_out, const void *param_data, unsigned long param_size);

#endif /* GTR_External_h */
