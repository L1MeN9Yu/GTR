//
//  gtr_atomic.c
//
//  Created L1MeN9Yu on 2017/11/29.
//
//

#include <pthread.h>
#include "gtr_atomic.h"

static pthread_mutex_t atomic_lock = PTHREAD_MUTEX_INITIALIZER;

int gtr_atomic_int_get(volatile int *ptr) {
    int res;

    pthread_mutex_lock(&atomic_lock);
    res = *ptr;
    pthread_mutex_unlock(&atomic_lock);

    return res;
}

void gtr_atomic_int_set(volatile int *ptr, int val) {
    pthread_mutex_lock(&atomic_lock);
    *ptr = val;
    pthread_mutex_unlock(&atomic_lock);
}

int gtr_atomic_int_add_and_fetch(volatile int *ptr, int inc) {
    int res;

    pthread_mutex_lock(&atomic_lock);
    *ptr += inc;
    res = *ptr;
    pthread_mutex_unlock(&atomic_lock);

    return res;
}

unsigned int gtr_atomic_unsigned_int_add_and_fetch(volatile unsigned int *ptr, unsigned int inc) {
    unsigned int res;
    pthread_mutex_lock(&atomic_lock);
    *ptr += inc;
    res = *ptr;
    pthread_mutex_unlock(&atomic_lock);
    return res;
}

void *gtr_atomic_ptr_cas(void *volatile *ptr, void *old_value, void *new_value) {
    void *ret;
    pthread_mutex_lock(&atomic_lock);
    ret = *ptr;
    if (ret == old_value)
        *ptr = new_value;
    pthread_mutex_unlock(&atomic_lock);
    return ret;
}