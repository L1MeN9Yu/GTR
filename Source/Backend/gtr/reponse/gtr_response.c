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
#include "gtr_curl_map.h"

//-------------------------------------//
const char *const BASE_INFO = "BASE_INFO";
const char *const RESPONSE_CODE = "RESPONSE_CODE";
const char *const PROTOCOL = "PROTOCOL";
const char *const SCHEME = "SCHEME";
const char *const EFFECTIVE_URL = "EFFECTIVE_URL";
const char *const CONNECT_CODE = "CONNECT_CODE";
const char *const HTTP_VERSION = "HTTP_VERSION";
const char *const REDIRECT_COUNT = "REDIRECT_COUNT";
const char *const REDIRECT_URL = "REDIRECT_URL";
const char *const CONNECT_COUNT = "CONNECT_COUNT";
const char *const CONDITION_UNMET = "CONDITION_UNMET";
//-------------------------------------//
const char *const TIME_INFO = "TIME_INFO";
const char *const NAME_LOOKUP_TIME = "NAME_LOOKUP_TIME";
const char *const CONNECT_TIME = "CONNECT_TIME";
const char *const APP_CONNECT_TIME = "APP_CONNECT_TIME";
const char *const PRE_TRANSFER_TIME = "PRE_TRANSFER_TIME";
const char *const START_TRANSFER_TIME = "START_TRANSFER_TIME";
const char *const TOTAL_TIME = "TOTAL_TIME";
const char *const REDIRECT_TIME = "REDIRECT_TIME";
//-------------------------------------//
const char *const SIZE_INFO = "SIZE_INFO";
const char *const UPLOAD_SIZE = "UPLOAD_SIZE";
const char *const DOWNLOAD_SIZE = "DOWNLOAD_SIZE";
const char *const HEADER_SIZE = "HEADER_SIZE";
const char *const REQUEST_SIZE = "REQUEST_SIZE";
//-------------------------------------//
const char *const SPEED_INFO = "SPEED_INFO";
const char *const UPLOAD_SPEED = "UPLOAD_SPEED";
const char *const DOWNLOAD_SPEED = "DOWNLOAD_SPEED";
//-------------------------------------//
const char *const SSL_INFO = "SSL_INFO";
const char *const SSL_VERIFY_RESULT = "SSL_VERIFY_RESULT";
const char *const SSL_ENGINES = "SSL_ENGINES";
//-------------------------------------//
const char *const SOCKET_INFO = "SOCKET_INFO";
const char *const PRIMARY_IP = "PRIMARY_IP";
const char *const PRIMARY_PORT = "PRIMARY_PORT";
const char *const LOCAL_IP = "LOCAL_IP";
const char *const LOCAL_PORT = "LOCAL_PORT";
//-------------------------------------//
const char *const COOKIE_LIST = "COOKIE_LIST";

//-------------------------------------//
static void base_info(cJSON *root, CURL *curl, gtr_core_data_task *data_task);

static void time_info(cJSON *root, CURL *curl, gtr_core_data_task *data_task);

static void size_info(cJSON *root, CURL *curl, gtr_core_data_task *data_task);

static void speed_info(cJSON *root, CURL *curl, gtr_core_data_task *data_task);

static void ssl_info(cJSON *root, CURL *curl, gtr_core_data_task *data_task);

static void socket_info(cJSON *root, CURL *curl, gtr_core_data_task *data_task);

static void cookie_list(cJSON *root, CURL *curl, gtr_core_data_task *data_task);

//-------------------------------------//
int gtr_get_data_task_response_info(CURL *curl, gtr_core_data_task *data_task, char **result, size_t *result_size) {
    cJSON *root = cJSON_CreateObject();

    base_info(root, curl, data_task);

    time_info(root, curl, data_task);

    size_info(root, curl, data_task);

    speed_info(root, curl, data_task);

    ssl_info(root, curl, data_task);

    socket_info(root, curl, data_task);

    cookie_list(root, curl, data_task);

    *result = cJSON_PrintUnformatted(root);
    *result_size = strlen(*result);

    cJSON_Delete(root);
    return 0;
}

static void base_info(cJSON *root, CURL *curl, gtr_core_data_task *data_task) {
    if (!data_task->response_info_options.base) {return;}

    cJSON *base_info = cJSON_CreateObject();

    long http_response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
    cJSON_AddItemToObject(base_info, RESPONSE_CODE, cJSON_CreateNumber(http_response_code));

    long protocol;
    curl_easy_getinfo(curl, CURLINFO_PROTOCOL, &protocol);
    const char *protocol_name = gtr_curl_map_protocol(protocol);
    cJSON_AddItemToObject(base_info, PROTOCOL, cJSON_CreateString(protocol_name ?: ""));

    char *scheme = NULL;
    curl_easy_getinfo(curl, CURLINFO_SCHEME, &scheme);
    cJSON_AddItemToObject(base_info, SCHEME, cJSON_CreateString(scheme ?: ""));

    char *effective_url = NULL;
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    cJSON_AddItemToObject(base_info, EFFECTIVE_URL, cJSON_CreateString(effective_url ?: ""));

    long connect_code;
    curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &connect_code);
    cJSON_AddItemToObject(base_info, CONNECT_CODE, cJSON_CreateNumber(connect_code));

    long http_version;
    curl_easy_getinfo(curl, CURLINFO_HTTP_VERSION, &http_version);
    const char *http_version_name = gtr_curl_map_http_version(http_version);
    cJSON_AddItemToObject(base_info, HTTP_VERSION, cJSON_CreateString(http_version_name ?: ""));

    long redirect_count;
    curl_easy_getinfo(curl, CURLINFO_REDIRECT_COUNT, &redirect_count);
    cJSON_AddItemToObject(base_info, REDIRECT_COUNT, cJSON_CreateNumber(redirect_count));

    char *redirect_url = NULL;
    curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &redirect_url);
    cJSON_AddItemToObject(base_info, REDIRECT_URL, cJSON_CreateString(redirect_url ?: ""));

    long connect_count;
    curl_easy_getinfo(curl, CURLINFO_NUM_CONNECTS, &connect_count);
    cJSON_AddItemToObject(base_info, CONNECT_COUNT, cJSON_CreateNumber(connect_count));

    long condition_unmet = 0;
    curl_easy_getinfo(curl, CURLINFO_CONDITION_UNMET, &condition_unmet);
    cJSON_AddItemToObject(base_info, CONDITION_UNMET, cJSON_CreateNumber(condition_unmet));

    cJSON_AddItemToObject(root, BASE_INFO, base_info);
}

static void time_info(cJSON *root, CURL *curl, gtr_core_data_task *data_task) {
    if (!data_task->response_info_options.time) {return;}

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

static void size_info(cJSON *root, CURL *curl, gtr_core_data_task *data_task) {
    if (!data_task->response_info_options.size) {return;}

    cJSON *size_info = cJSON_CreateObject();

    long upload_size;
    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &upload_size);
    cJSON_AddItemToObject(size_info, UPLOAD_SIZE, cJSON_CreateNumber(upload_size));

    long download_size;
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &download_size);
    cJSON_AddItemToObject(size_info, DOWNLOAD_SIZE, cJSON_CreateNumber(download_size));

    long header_size;
    curl_easy_getinfo(curl, CURLINFO_HEADER_SIZE, &header_size);
    cJSON_AddItemToObject(size_info, HEADER_SIZE, cJSON_CreateNumber(header_size));

    long request_size;
    curl_easy_getinfo(curl, CURLINFO_REQUEST_SIZE, &request_size);
    cJSON_AddItemToObject(size_info, REQUEST_SIZE, cJSON_CreateNumber(request_size));

    cJSON_AddItemToObject(root, SIZE_INFO, size_info);
}

static void speed_info(cJSON *root, CURL *curl, gtr_core_data_task *data_task) {
    if (!data_task->response_info_options.speed) {return;}

    cJSON *speed_info = cJSON_CreateObject();

    long upload_speed;
    curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD_T, &upload_speed);
    cJSON_AddItemToObject(speed_info, UPLOAD_SPEED, cJSON_CreateNumber(upload_speed));

    long download_speed;
    curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD_T, &download_speed);
    cJSON_AddItemToObject(speed_info, DOWNLOAD_SPEED, cJSON_CreateNumber(download_speed));

    cJSON_AddItemToObject(root, SPEED_INFO, speed_info);
}

static void ssl_info(cJSON *root, CURL *curl, gtr_core_data_task *data_task) {
    if (!data_task->response_info_options.ssl) {return;}

    cJSON *ssl_info = cJSON_CreateObject();

    long verify_result;
    curl_easy_getinfo(curl, CURLINFO_PROXY_SSL_VERIFYRESULT, &verify_result);
    cJSON_AddItemToObject(ssl_info, SSL_VERIFY_RESULT, cJSON_CreateBool(verify_result == 1));

    struct curl_slist *engines = NULL;
    curl_easy_getinfo(curl, CURLINFO_SSL_ENGINES, &engines);

    cJSON *engine_list = cJSON_CreateArray();

    struct curl_slist *current = engines;
    while (current != NULL) {
        if (current->data) {
            cJSON_AddItemToArray(engine_list, cJSON_CreateString(current->data ?: ""));
        }
        current = current->next;
    }

    cJSON_AddItemToObject(ssl_info, SSL_ENGINES, engine_list);

    cJSON_AddItemToObject(root, SSL_INFO, ssl_info);

    if (engines) {
        curl_slist_free_all(engines);
    }
}

static void socket_info(cJSON *root, CURL *curl, gtr_core_data_task *data_task) {
    if (!data_task->response_info_options.socket) {return;}

    cJSON *socket_info = cJSON_CreateObject();

    char *primary_ip;
    curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &primary_ip);
    cJSON_AddItemToObject(socket_info, PRIMARY_IP, cJSON_CreateString(primary_ip ?: ""));

    long primary_port;
    curl_easy_getinfo(curl, CURLINFO_PRIMARY_PORT, &primary_port);
    cJSON_AddItemToObject(socket_info, PRIMARY_PORT, cJSON_CreateNumber(primary_port));

    char *local_ip;
    curl_easy_getinfo(curl, CURLINFO_LOCAL_IP, &local_ip);
    cJSON_AddItemToObject(socket_info, LOCAL_IP, cJSON_CreateString(local_ip ?: ""));

    long local_port;
    curl_easy_getinfo(curl, CURLINFO_LOCAL_PORT, &local_port);
    cJSON_AddItemToObject(socket_info, LOCAL_PORT, cJSON_CreateNumber(local_port));

    cJSON_AddItemToObject(root, SOCKET_INFO, socket_info);
}

static void cookie_list(cJSON *root, CURL *curl, gtr_core_data_task *data_task) {
    if (!data_task->response_info_options.cookie) {return;}

    cJSON *cookie_list = cJSON_CreateArray();

    struct curl_slist *cookies = NULL;
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
    if (cookies) {
        struct curl_slist *current = cookies;
        while (current) {
            cJSON_AddItemToArray(cookie_list, cJSON_CreateString(current->data ?: ""));
            current = current->next;
        }
        curl_slist_free_all(cookies);
    }

    cJSON_AddItemToObject(root, COOKIE_LIST, cookie_list);
}
