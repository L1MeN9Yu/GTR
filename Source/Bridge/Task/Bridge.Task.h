//
//  GTR.External.h
//  GTR
//
//  Created L1MeN9Yu on 2018/8/29.
//  Copyright © 2018 limengyu.top. All rights reserved.
//

#ifndef GTR_External_h
#define GTR_External_h

#include <stdbool.h>


typedef struct gtr_core_data_task gtr_core_data_task;

__attribute__((nonnull(1), used))
extern gtr_core_data_task *gtr_data_task_create(unsigned *task_id, const char *url, const char *headers);

__attribute__((nonnull(1), used))
extern void gtr_data_task_config_parameters(gtr_core_data_task *data_task, const char *method, const void *param_data, unsigned long param_size);

__attribute__((nonnull(1), used))
extern void gtr_data_task_add_form_data(gtr_core_data_task *data_task, int type, const char *name, const char *value);

__attribute__((nonnull(1), used))
extern void gtr_data_task_config_options(gtr_core_data_task *data_task, bool is_debug, unsigned int time_out, long max_redirects);

__attribute__((nonnull(1), used))
extern void gtr_data_task_config_response_info_options(gtr_core_data_task *data_task, bool base, bool time, bool size, bool speed, bool ssl, bool socket, bool cookie);

__attribute__((nonnull(1), used))
extern void gtr_data_task_config_speed(gtr_core_data_task *data_task, long max_receive_speed, long max_send_speed, long low_speed_limit, long low_speed_time);

__attribute__((nonnull(1), used))
extern void gtr_data_task_config_proxy(gtr_core_data_task *data_task, const char *url, long port);

__attribute__((nonnull(1), used))
extern void gtr_data_task_start(gtr_core_data_task *data_task);

/**
 * 发起下载请求
 * @param task_id 任务id
 * @param url 请求地址
 * @param file_path 下载到本地的路径
 * @param headers 请求的Header
 * @param time_out 请求的超时时间
 */
extern void gtr_download(unsigned int *task_id, const char *url, const char *file_path, const char *headers, unsigned int time_out, long speed_limit);

/**
 * 取消请求
 * @param task_id task_id
 */
extern void gtr_cancel(unsigned int task_id);

#endif /* GTR_External_h */
