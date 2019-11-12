//
//  gtr_response.c
//  GTR
//
//  Created by Mengyu Li on 2019/11/12.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#include "gtr_response.h"
#include <cJSON/cJSON.h>
#include <memory.h>

const char *const RESPONSE_CODE         = "RESPONSE_CODE";
const char *const TIME_INFO             = "TIME_INFO";
const char *const NAME_LOOKUP_TIME      = "NAME_LOOKUP_TIME";
const char *const CONNECT_TIME          = "CONNECT_TIME";
const char *const APP_CONNECT_TIME      = "APP_CONNECT_TIME";
const char *const PRE_TRANSFER_TIME     = "PRE_TRANSFER_TIME";
const char *const START_TRANSFER_TIME   = "START_TRANSFER_TIME";
const char *const TOTAL_TIME            = "TOTAL_TIME";
const char *const REDIRECT_TIME         = "REDIRECT_TIME";

int gtr_get_data_task_response_info(CURL *curl, gtr_core_data_task *data_task, char **result, size_t *result_size) {
    cJSON *root = cJSON_CreateObject();

    {
        long http_response_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
        cJSON_AddItemToObject(root, RESPONSE_CODE, cJSON_CreateNumber(http_response_code));
    }

    {
        cJSON *time_info = cJSON_CreateObject();
        double name_lookup_time = 0;
        curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &name_lookup_time);
        cJSON_AddItemToObject(time_info, NAME_LOOKUP_TIME, cJSON_CreateNumber(name_lookup_time));

        double connect_time;
        curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &connect_time);
        cJSON_AddItemToObject(time_info, CONNECT_TIME, cJSON_CreateNumber(connect_time));

        double app_connect_time;
        curl_easy_getinfo(curl, CURLINFO_APPCONNECT_TIME, &app_connect_time);
        cJSON_AddItemToObject(time_info, APP_CONNECT_TIME, cJSON_CreateNumber(app_connect_time));

        double pre_transfer_time;
        curl_easy_getinfo(curl, CURLINFO_PRETRANSFER_TIME, &pre_transfer_time);
        cJSON_AddItemToObject(time_info, PRE_TRANSFER_TIME, cJSON_CreateNumber(pre_transfer_time));

        double start_transfer_time;
        curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &start_transfer_time);
        cJSON_AddItemToObject(time_info, START_TRANSFER_TIME, cJSON_CreateNumber(start_transfer_time));

        double total_time;
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        cJSON_AddItemToObject(time_info, TOTAL_TIME, cJSON_CreateNumber(total_time));

        double redirect_time;
        curl_easy_getinfo(curl, CURLINFO_REDIRECT_TIME, &redirect_time);
        cJSON_AddItemToObject(time_info, REDIRECT_TIME, cJSON_CreateNumber(redirect_time));

        cJSON_AddItemToObject(root, TIME_INFO, time_info);
    }
    long condition_unmet = 0;
    curl_easy_getinfo(curl, CURLINFO_CONDITION_UNMET, &condition_unmet);

    *result = cJSON_Print(root);
    *result_size = strlen(*result);

    cJSON_Delete(root);
    return 0;
}
