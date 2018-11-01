//
//  gtr_core.h
//  GTR
//
//  Created L1MeN9Yu on 2018/9/1.
//  Copyright © 2018 limengyu.top. All rights reserved.
//

#ifndef gtr_core_h
#define gtr_core_h

#include <stdio.h>
#include "gtr_core_request.h"

/**
 * 初始化,!!!非线程安全!!!,全局应该只执行一次
 * @param user_agent user_agent
 * @param log_callback log回调函数
 */
void
gtr_core_init(
        const char *user_agent,
        void *log_callback
);

/**
 * dispose
 */
void __unused
gtr_core_dispose(
        void
);

/**
 * 开启代理
 * @param url url
 * @param port 端口
 */
void
gtr_core_open_proxy(
        const char *url,
        unsigned int port
);

/**
 * 关闭代理
 */
void
gtr_core_close_proxy(
        void
);

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
void gtr_core_add_request(
        unsigned int *task_id,
        gtr_core_request_type type,
        const char *url,
        const char *header,
        unsigned int time_out,
        const void *request_data,
        unsigned long request_data_size,
        void *succeed_callback,
        void *failure_callback
);

void gtr_core_add_download_request(
        unsigned int *task_id,
        const char *url,
        const char *file_path,
        const char *header,
        unsigned int time_out,
        void *progress_callback,
        void *succeed_callback,
        void *failure_callback
);

#endif /* gtr_core_h */
