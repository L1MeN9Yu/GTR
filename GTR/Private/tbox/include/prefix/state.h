/*!The Treasure Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2009 - 2018, TBOOX Open Source Group.
 *
 * @author      ruki
 * @file        state.h
 *
 */
#ifndef TB_PREFIX_STATE_H
#define TB_PREFIX_STATE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "config.h"
#include "type.h"
#include "keyword.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

/// the state define
#define TB_STATE_DEFINE(type, state)    (((type) << 16) | (state))

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the state type enum
typedef enum __tb_state_type_e
{
    TB_STATE_TYPE_NONE      = 0
,   TB_STATE_TYPE_SOCK      = 1
,   TB_STATE_TYPE_HTTP      = 2
,   TB_STATE_TYPE_DATABASE  = 3
,   TB_STATE_TYPE_SYSERROR  = 4

}tb_state_type_e;

/// the state enum
typedef enum __tb_state_e
{
    TB_STATE_OK                         = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 0)
,   TB_STATE_END                        = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 1)
,   TB_STATE_DEAD                       = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 2)
,   TB_STATE_READY                      = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 3)
,   TB_STATE_FAILED                     = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 4)
,   TB_STATE_KILLED                     = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 5)
,   TB_STATE_PAUSED                     = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 6)
,   TB_STATE_CLOSED                     = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 7)
,   TB_STATE_OPENED                     = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 8)
,   TB_STATE_OPENING                    = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 9)
,   TB_STATE_SUSPEND                    = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 10)
,   TB_STATE_RUNNING                    = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 11)
,   TB_STATE_PENDING                    = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 12)
,   TB_STATE_WAITING                    = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 13)
,   TB_STATE_WORKING                    = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 14)
,   TB_STATE_KILLING                    = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 15)
,   TB_STATE_PAUSING                    = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 16)
,   TB_STATE_EXITING                    = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 17)
,   TB_STATE_TIMEOUT                    = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 18)
,   TB_STATE_FINISHED                   = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 19)
,   TB_STATE_WAIT_FAILED                = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 20)
,   TB_STATE_POST_FAILED                = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 21)
,   TB_STATE_NOT_SUPPORTED              = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 22)
,   TB_STATE_UNKNOWN_ERROR              = TB_STATE_DEFINE(TB_STATE_TYPE_NONE, 23)

,   TB_STATE_SOCK_DNS_FAILED            = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 1)
,   TB_STATE_SOCK_CONNECT_FAILED        = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 2)
,   TB_STATE_SOCK_CONNECT_TIMEOUT       = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 3)
,   TB_STATE_SOCK_RECV_TIMEOUT          = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 4)
,   TB_STATE_SOCK_SEND_TIMEOUT          = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 5)
,   TB_STATE_SOCK_RECV_FAILED           = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 6)
,   TB_STATE_SOCK_SEND_FAILED           = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 7)
,   TB_STATE_SOCK_OPEN_FAILED           = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 8)
,   TB_STATE_SOCK_UNKNOWN_ERROR         = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 9)
,   TB_STATE_SOCK_SSL_FAILED            = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 10)
,   TB_STATE_SOCK_SSL_TIMEOUT           = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 11)
,   TB_STATE_SOCK_SSL_WANT_READ         = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 12)
,   TB_STATE_SOCK_SSL_WANT_WRIT         = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 13)
,   TB_STATE_SOCK_SSL_WAIT_FAILED       = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 14)
,   TB_STATE_SOCK_SSL_READ_FAILED       = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 15)
,   TB_STATE_SOCK_SSL_WRIT_FAILED       = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 16)
,   TB_STATE_SOCK_SSL_NOT_SUPPORTED     = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 17)
,   TB_STATE_SOCK_SSL_UNKNOWN_ERROR     = TB_STATE_DEFINE(TB_STATE_TYPE_SOCK, 18)

,   TB_STATE_HTTP_RESPONSE_100          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 1)
,   TB_STATE_HTTP_RESPONSE_101          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 2)
,   TB_STATE_HTTP_RESPONSE_102          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 3)
,   TB_STATE_HTTP_RESPONSE_200          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 4)
,   TB_STATE_HTTP_RESPONSE_204          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 5)
,   TB_STATE_HTTP_RESPONSE_300          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 6)
,   TB_STATE_HTTP_RESPONSE_301          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 7)
,   TB_STATE_HTTP_RESPONSE_302          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 8)
,   TB_STATE_HTTP_RESPONSE_303          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 9)
,   TB_STATE_HTTP_RESPONSE_304          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 10)
,   TB_STATE_HTTP_RESPONSE_305          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 11)
,   TB_STATE_HTTP_RESPONSE_306          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 12)
,   TB_STATE_HTTP_RESPONSE_307          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 13)
,   TB_STATE_HTTP_RESPONSE_400          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 14)
,   TB_STATE_HTTP_RESPONSE_401          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 15)
,   TB_STATE_HTTP_RESPONSE_402          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 16)
,   TB_STATE_HTTP_RESPONSE_403          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 17)
,   TB_STATE_HTTP_RESPONSE_404          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 18)
,   TB_STATE_HTTP_RESPONSE_405          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 19)
,   TB_STATE_HTTP_RESPONSE_406          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 20)
,   TB_STATE_HTTP_RESPONSE_407          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 21)
,   TB_STATE_HTTP_RESPONSE_408          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 22)
,   TB_STATE_HTTP_RESPONSE_409          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 23)
,   TB_STATE_HTTP_RESPONSE_410          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 24)
,   TB_STATE_HTTP_RESPONSE_411          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 25)
,   TB_STATE_HTTP_RESPONSE_412          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 26)
,   TB_STATE_HTTP_RESPONSE_413          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 27)
,   TB_STATE_HTTP_RESPONSE_414          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 28)
,   TB_STATE_HTTP_RESPONSE_415          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 29)
,   TB_STATE_HTTP_RESPONSE_416          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 30)
,   TB_STATE_HTTP_RESPONSE_500          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 31)
,   TB_STATE_HTTP_RESPONSE_501          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 32)
,   TB_STATE_HTTP_RESPONSE_502          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 33)
,   TB_STATE_HTTP_RESPONSE_503          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 34)
,   TB_STATE_HTTP_RESPONSE_504          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 35)
,   TB_STATE_HTTP_RESPONSE_505          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 36)
,   TB_STATE_HTTP_RESPONSE_506          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 37)
,   TB_STATE_HTTP_RESPONSE_507          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 38)
,   TB_STATE_HTTP_RESPONSE_UNK          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 39)
,   TB_STATE_HTTP_RESPONSE_NUL          = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 40)
,   TB_STATE_HTTP_REQUEST_FAILED        = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 41)
,   TB_STATE_HTTP_REDIRECT_FAILED       = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 42)
,   TB_STATE_HTTP_POST_FAILED           = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 43)
,   TB_STATE_HTTP_RANGE_INVALID         = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 44)
,   TB_STATE_HTTP_GZIP_NOT_SUPPORTED    = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 45)
,   TB_STATE_HTTP_UNKNOWN_ERROR         = TB_STATE_DEFINE(TB_STATE_TYPE_HTTP, 46)

,   TB_STATE_DATABASE_NO_SUCH_TABLE     = TB_STATE_DEFINE(TB_STATE_TYPE_DATABASE, 1)
,   TB_STATE_DATABASE_NO_SUCH_FIELD     = TB_STATE_DEFINE(TB_STATE_TYPE_DATABASE, 2)
,   TB_STATE_DATABASE_NO_SUCH_DATABASE  = TB_STATE_DEFINE(TB_STATE_TYPE_DATABASE, 3)
,   TB_STATE_DATABASE_ACCESS_DENIED     = TB_STATE_DEFINE(TB_STATE_TYPE_DATABASE, 4)
,   TB_STATE_DATABASE_PARSE_ERROR       = TB_STATE_DEFINE(TB_STATE_TYPE_DATABASE, 5)
,   TB_STATE_DATABASE_VALUE_COUNT_ERROR = TB_STATE_DEFINE(TB_STATE_TYPE_DATABASE, 6)
,   TB_STATE_DATABASE_UNKNOWN_HOST      = TB_STATE_DEFINE(TB_STATE_TYPE_DATABASE, 7)
,   TB_STATE_DATABASE_UNKNOWN_ERROR     = TB_STATE_DEFINE(TB_STATE_TYPE_DATABASE, 8)

,   TB_STATE_SYSERROR_NOT_PERM          = TB_STATE_DEFINE(TB_STATE_TYPE_SYSERROR, 1)
,   TB_STATE_SYSERROR_NOT_FILEDIR       = TB_STATE_DEFINE(TB_STATE_TYPE_SYSERROR, 2)
,   TB_STATE_SYSERROR_UNKNOWN_ERROR     = TB_STATE_DEFINE(TB_STATE_TYPE_SYSERROR, 3)

}tb_state_e;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! the state string
 *
 * @param state     the state
 *
 * @return          the state string
 */
tb_char_t const*    tb_state_cstr(tb_size_t state);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


