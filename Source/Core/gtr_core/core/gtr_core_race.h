//
//  gtr_core_race.h
//  GTR
//
//  Created L1MeN9Yu on 2018/9/2.
//  Copyright © 2018 limengyu.top. All rights reserved.
//

#ifndef gtr_core_request_h
#define gtr_core_request_h

#include <stdio.h>
#include <stdbool.h>
#include <curl/curl.h>

/**
 *
 */
typedef enum gtr_core_race_type {
    gtr_core_request_type_get,
    gtr_core_request_type_post,
    gtr_core_request_type_put,
    gtr_core_request_type_download,
    gtr_core_request_type_upload
} gtr_core_race_type;

/**
 *
 */
typedef struct gtr_core_race_response_header {
    char *data;
    unsigned long size;
} gtr_core_race_response_header;

/**
 *
 */
typedef struct gtr_core_race_request_body {
    char *data;
    unsigned long size;
    unsigned long size_left;
} gtr_core_race_request_body;

/**
 *
 */
typedef struct gtr_core_race_response_body {
    char *data;
    unsigned long size;
} gtr_core_race_response_body;

/**
 *
 */
typedef struct gtr_core_race_download_data {
    char *file_path;
    unsigned int task_id;

    void (*on_progress)(unsigned int task_id, unsigned long long now, unsigned long long total);
} gtr_core_race_download_data;

/**
 * 请求
 */
typedef struct gtr_core_race {
    unsigned int task_id;
    bool is_cancel;
    gtr_core_race_type request_type;
    char *url;
    char *header;
    unsigned int time_out;
    long speed_limit;
    gtr_core_race_request_body *request_data;
    gtr_core_race_download_data *download_data;

    /**
     * succeed callback
     * @param task_id
     * @param http_response_code
     * @param header_data
     * @param header_data_size
     * @param body_data
     * @param body_data_size
     */
    void (*on_succeed)(unsigned int task_id, long http_response_code, void *header_data, unsigned long header_data_size, void *body_data, unsigned long body_data_size);

    /**
     *
     * @param task_id
     * @param http_response_code
     * @param error_code
     * @param error_message
     */
    void (*on_failed)(unsigned int task_id, long http_response_code, CURLcode error_code, const char *error_message);
} gtr_core_race;


#endif /* gtr_core_request_h */
