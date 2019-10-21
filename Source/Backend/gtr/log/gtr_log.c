//
// Created by Mengyu Li on 2018-10-18.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

#include <zconf.h>
#include <stdio.h>
#include "gtr_log.h"

#define LOG_MAX_BUF_SIZE 512

void (*global_gtr_core_log_callback)(unsigned int flag, char *log_message);

void config_log_callback(void *log_callback) {
    global_gtr_core_log_callback = log_callback;
}

void gtr_core_log(gtr_log_flag flag, const char *format, ...) {
    if (global_gtr_core_log_callback) {
        static char buffer[LOG_MAX_BUF_SIZE];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, LOG_MAX_BUF_SIZE, format, args);
        va_end(args);
        global_gtr_core_log_callback(flag, buffer);
    }
}
