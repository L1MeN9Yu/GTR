//
//  Bridge.Core.h
//  GTR
//
//  Created by Mengyu Li on 2019/10/18.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#ifndef Bridge_Core_h
#define Bridge_Core_h

#include <stdbool.h>

/**
 * 初始化GTR,生命周期内只执行一次
 * @param user_agent 全局user agent
 * @param cylinder_count 线程池数量
 */
extern void gtr_init(const char *user_agent, unsigned int cylinder_count);


#endif /* Bridge_Core_h */
