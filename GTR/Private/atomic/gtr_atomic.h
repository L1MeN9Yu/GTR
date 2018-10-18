//
//  mcf_atomic.h
//
//  Created by L1MeN9Yu on 2017/11/29.
//
//
#ifndef gtr_atomic_h
#define gtr_atomic_h

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Load the current value stored in an atomic integer.
 *
 * @param ptr atomic integer
 * @return the current value of the atomic integer
 * @note This acts as a memory barrier.
 */
int gtr_atomic_int_get(volatile int *ptr);

/**
 * Store a new value in an atomic integer.
 *
 * @param ptr atomic integer
 * @param val the value to store in the atomic integer
 * @note This acts as a memory barrier.
 */
void gtr_atomic_int_set(volatile int *ptr, int val);

/**
 * Add a value to an atomic integer.
 *
 * @param ptr atomic integer
 * @param inc the value to add to the atomic integer (may be negative)
 * @return the new value of the atomic integer.
 * @note This does NOT act as a memory barrier. This is primarily
 *       intended for reference counting.
 */
int gtr_atomic_int_add_and_fetch(volatile int *ptr, int inc);

/**
 * Add a value to an atomic unsigned integer.
 *
 * @param ptr ptr atomic unsigned integer
 * @param inc inc the value to add to the atomic unsigned integer (may be negative)
 * @return the new value of the atomic unsigned integer.
 * @note This does NOT act as a memory barrier. This is primarily
 *       intended for reference counting.
 */
unsigned int gtr_atomic_unsigned_int_add_and_fetch(volatile unsigned int *ptr, unsigned int inc);

/**
 * Atomic pointer compare and swap.
 *
 * @param ptr pointer to the pointer to operate on
 * @param old_value do the swap if the current value of *ptr equals to old value
 * @param new_value value to replace *ptr with
 * @return the value of *ptr before comparison
 */
void *gtr_atomic_ptr_cas(void *volatile *ptr, void *old_value, void *new_value);

#endif /* gtr_atomic_h */


#ifdef __cplusplus
}
#endif
