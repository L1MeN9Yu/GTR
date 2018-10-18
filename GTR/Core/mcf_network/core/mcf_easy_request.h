//
//  mcf_easy_request.h
//  GTR
//
//  Created L1MeN9Yu on 2018/9/2.
//  Copyright © 2018 limengyu.top. All rights reserved.
//

#ifndef mcf_easy_request_h
#define mcf_easy_request_h

#include <stdio.h>
#include <stdbool.h>
#include "curl.h"

typedef enum {
    mcf_easy_request_type_get,
    mcf_easy_request_type_post,
    mcf_easy_request_type_put,
    mcf_easy_request_type_download,
    mcf_easy_request_type_upload
} mcf_easy_request_type;

typedef struct {
    char *data;
    unsigned long size;
    unsigned long size_left;
} mcf_easy_request_request_data;

typedef struct {
    char *response_data;
    unsigned long response_data_size;
} mcf_easy_request_response_data;

typedef struct {

} mcf_easy_request_download_data;

typedef struct {
    unsigned int task_id;
    bool is_cancel;
    mcf_easy_request_type request_type;
    char *url;
    char *header;
    unsigned int time_out;
    mcf_easy_request_request_data *request_data;

    void (*on_succeed)(unsigned int task_id, long http_response_code, void *data, unsigned long data_size);

    void (*on_failed)(unsigned int task_id, long http_response_code, CURLcode error_code, const char *error_message);
} mcf_easy_request;


#endif /* mcf_easy_request_h */
