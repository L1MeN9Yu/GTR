//
//  mcf_easy_header_helper.h
//  GTR
//
//  Created L1MeN9Yu on 2018/9/2.
//  Copyright © 2018 limengyu.top. All rights reserved.
//

#ifndef mcf_easy_header_helper_h
#define mcf_easy_header_helper_h

#include <stdio.h>
#include "curl.h"

struct curl_slist *mcf_easy_add_custom_headers(const char *headers_json);

#endif /* mcf_easy_header_helper_h */
