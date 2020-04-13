//
//  share.c
//  GTR
//
//  Created by Mengyu Li on 2020/4/13.
//  Copyright © 2020 limengyu.top. All rights reserved.
//

#include <pthread.h>
#include "share.h"

static pthread_mutex_t lock;

static void lock_cb(
        CURL *__unused handle,
        curl_lock_data __unused data,
        curl_lock_access __unused access,
        void *__unused user_data) {
    pthread_mutex_lock(&lock);
}

static void unlock_cb(
        CURL * __unused handle,
        curl_lock_data __unused data,
        void * __unused user_data) {
    pthread_mutex_unlock(&lock);
}

static void init_locks(void) {
    pthread_mutex_init(&lock, NULL);
}

void gtr_share_init(void) {
    init_locks();
    share = curl_share_init();
    curl_share_setopt(share, CURLSHOPT_LOCKFUNC, lock_cb);
    curl_share_setopt(share, CURLSHOPT_UNLOCKFUNC, unlock_cb);
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_CONNECT);
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
}

void gtr_share_dispose(void) {
    pthread_mutex_destroy(&lock);
    curl_share_cleanup(share);
}
