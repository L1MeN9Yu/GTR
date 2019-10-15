//
//  gtr_core_header_helper.c
//  GTR
//
//  Created L1MeN9Yu on 2018/9/2.
//  Copyright © 2018 limengyu.top. All rights reserved.
//

#include <string.h>
#include "gtr_header.h"

struct curl_slist *gtr_core_add_custom_headers(const char *headers_json) {
    struct curl_slist *header = NULL;
    if (headers_json != NULL && strlen(headers_json) > 2) {
        char *walker_p = (char *) headers_json;
        char header_buf[1024];
        if (*walker_p != '{' || *(walker_p + strlen(headers_json) - 1) != '}') {
            return NULL;
        }

        while (walker_p != NULL) {
            int i = 0;
            char *end_p = strstr(walker_p + 1, ",\"");
            if (end_p == NULL) {
                end_p = strstr(walker_p, "}");
                if (end_p == NULL) {
                    return NULL;
                }
            }
            walker_p++;

            //剔除空字符串("")
            do {
                if (*walker_p != '\"') {
                    header_buf[i++] = *walker_p;
                }

            } while (++walker_p != end_p);
            header_buf[i] = '\0';
            header = curl_slist_append(header, header_buf);
            walker_p = strstr(end_p, ",\"");
        }
    }
    return header;
}