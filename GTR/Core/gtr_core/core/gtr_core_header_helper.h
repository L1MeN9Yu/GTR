//
//  gtr_core_header_helper.h
//  GTR
//
//  Created L1MeN9Yu on 2018/9/2.
//  Copyright © 2018 limengyu.top. All rights reserved.
//

#ifndef gtr_core_header_helper_h
#define gtr_core_header_helper_h

#include <stdio.h>
#include "curl.h"

struct curl_slist *gtr_core_add_custom_headers(const char *headers_json);

#endif /* gtr_core_header_helper_h */
