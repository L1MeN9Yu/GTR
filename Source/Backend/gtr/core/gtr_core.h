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
#include "gtr_race.h"

/**
 * 初始化,!!!非线程安全!!!,全局应该只执行一次
 * @param user_agent user_agent
 * @param log_callback log回调函数
 */
void gtr_core_init(const char *user_agent, void *log_callback, unsigned int cylinder_count);

/**
 * dispose
 */
void __unused gtr_core_dispose(void);

/**
 * data task start
 * @param core_data_task data task
 */
void gtr_core_data_task_start(gtr_core_data_task *core_data_task);

#endif /* gtr_core_h */
