//
//  gtr_core_request.h
//  GTR
//
//  Created L1MeN9Yu on 2018/9/2.
//  Copyright © 2018 limengyu.top. All rights reserved.
//

#ifndef gtr_core_request_h
#define gtr_core_request_h

#include <stdio.h>
#include <stdbool.h>
#include "curl.h"

typedef enum {
    gtr_core_request_type_get,
    gtr_core_request_type_post,
    gtr_core_request_type_put,
    gtr_core_request_type_download,
    gtr_core_request_type_upload
} gtr_core_request_type;

typedef struct {
    char *data;
    unsigned long size;
    unsigned long size_left;
} gtr_core_request_request_data;

typedef struct {
    char *response_data;
    unsigned long response_data_size;
} gtr_core_request_response_data;

typedef struct {

} gtr_core_request_download_data;

typedef struct {
    unsigned int task_id;
    bool is_cancel;
    gtr_core_request_type request_type;
    char *url;
    char *header;
    unsigned int time_out;
    gtr_core_request_request_data *request_data;

    void (*on_succeed)(unsigned int task_id, long http_response_code, void *data, unsigned long data_size);

    void (*on_failed)(unsigned int task_id, long http_response_code, CURLcode error_code, const char *error_message);
} gtr_core_request;


#endif /* gtr_core_request_h */
