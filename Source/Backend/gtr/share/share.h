//
//  share.h
//  GTR
//
//  Created by Mengyu Li on 2020/4/13.
//  Copyright © 2020 limengyu.top. All rights reserved.
//

#ifndef share_h
#define share_h

#include <curl/curl.h>

static CURLSH *share;

void gtr_share_init(void);

void gtr_share_dispose(void);

#endif /* share_h */
