//
//  gtr_tbox_main.c
//  GTR
//
//  Created by Mengyu Li on 2019-04-06.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#include "gtr_tbox_main.h"
#include <tbox.h>

void gtr_tbox_setup(void) {
    int succeed = tb_init(tb_null, tb_null);
    if (succeed) {
        tb_trace_i("the tbox version = %s",TB_CONFIG_VERSION);
    }
}