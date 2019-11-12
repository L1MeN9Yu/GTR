//
//  gtr_response.h
//  GTR
//
//  Created by Mengyu Li on 2019/11/12.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#include <curl/curl.h>
#include "gtr_race.h"

#ifndef gtr_response_h
#define gtr_response_h

int gtr_get_data_task_response_info(CURL *curl, gtr_core_data_task *data_task, char **result, size_t *result_size);

#endif /* gtr_response_h */
