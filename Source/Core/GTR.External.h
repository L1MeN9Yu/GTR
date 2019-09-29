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

/**
 * 初始化GTR,生命周期内只执行一次
 * @param user_agent 全局user agent
 */
extern void gtr_init(const char *user_agent, unsigned int cylinder_count);

/**
 * 配置代理
 * @param enable 是否开启
 * @param url 代理的地址
 * @param port 代理的端口
 */
extern void gtr_proxy(bool enable, const char *url, unsigned int port);

/**
 * 发起GET请求
 * @param task_id 任务id
 * @param url 请求地址
 * @param headers 请求的Header
 * @param time_out 请求的超时时间
 */
extern void gtr_get(unsigned *task_id, const char *url, const char *headers, unsigned int time_out, long speed_limit);

/**
 * 发起POST请求
 * @param task_id 任务id
 * @param url 请求地址
 * @param headers 请求的Header
 * @param time_out 请求的超时时间
 * @param param_data 请求体的数据
 * @param param_size 请求体的数据大小
 */
extern void gtr_post(unsigned int *task_id, const char *url, const char *headers, unsigned int time_out, long speed_limit, const void *param_data, unsigned long param_size);

/**
 * 发起PUT请求
 * @param task_id 任务id
 * @param url 请求地址
 * @param headers 请求的Header
 * @param time_out 请求的超时时间
 * @param param_data 请求体的数据
 * @param param_size 请求体的数据大小
 */
extern void gtr_put(unsigned int *task_id, const char *url, const char *headers, unsigned int time_out, long speed_limit, const void *param_data, unsigned long param_size);

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
 * @param task_id
 */
extern void gtr_cancel(unsigned int task_id);

#endif /* GTR_External_h */
