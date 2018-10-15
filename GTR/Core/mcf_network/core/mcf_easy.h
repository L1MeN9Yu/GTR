//
//  mcf_easy.h
//  GTR
//
//  Created L1MeN9Yu on 2018/9/1.
//  Copyright © 2018 limengyu.top. All rights reserved.
//

#ifndef mcf_easy_h
#define mcf_easy_h

#include <stdio.h>
#include "mcf_easy_request.h"

/**
 * 初始化,!!!非线程安全!!!,全局应该只执行一次
 * @param user_agent user_agent
 * @param log_callback log回调函数
 */
void mcf_easy_init(const char *user_agent, void *log_callback);

void __unused mcf_easy_dispose(void);

/**
 * 开启代理
 * @param url url
 * @param port 端口
 */
void mcf_easy_open_proxy(const char *url, unsigned int port);

/**
 * 关闭代理
 */
void mcf_easy_close_proxy(void);

/**
 * 发起一个请求
 * @param task_id 任务ID指针
 * @param type 请求类型
 * @param url 请求地址
 * @param header 请求Header
 * @param time_out 请求超时时间
 * @param request_data 请求体
 * @param request_data_size 请求体大小
 * @param succeed_callback 请求成功回调函数
 * @param failure_callback 请求失败回调函数
 */
void mcf_easy_add_request(
        unsigned int *task_id,
        mcf_easy_request_type type,
        const char *url,
        const char *header,
        unsigned int time_out,
        const void *request_data,
        unsigned long request_data_size,
        void *succeed_callback,
        void *failure_callback
);

#endif /* mcf_easy_h */
