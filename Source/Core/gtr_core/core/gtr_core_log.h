//
// Created by Mengyu Li on 2018-10-18.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

#ifndef GTR_CORE_LOG_H
#define GTR_CORE_LOG_H

typedef enum {
    gtr_log_flag_trace = 0,
    gtr_log_flag_debug,
    gtr_log_flag_info,
    gtr_log_flag_warning,
    gtr_log_flag_error,
    gtr_log_flag_critical
} gtr_log_flag;

void config_log_callback(void *log_callback);

void gtr_core_log(gtr_log_flag flag, const char *format, ...);

#endif //GTR_CORE_LOG_H
