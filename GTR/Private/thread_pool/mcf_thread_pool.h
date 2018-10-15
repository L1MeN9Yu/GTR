#ifndef MCF_THREAD_POOL_H
#define MCF_THREAD_POOL_H

#ifdef __cplusplus
extern "C" {
#endif

/* =================================== API ======================================= */


typedef struct mcf_thread_pool_ *thread_pool;


/**
 * @brief  Initialize thread_pool
 *
 * Initializes a thread_pool. This function will not return until all
 * threads have initialized successfully.
 *
 * @example
 *
 *    ..
 *    thread_pool t_p;                     //First we declare a thread_pool
 *    t_p = thread_pool_init(4);               //then we initialize it to 4 threads
 *    ..
 *
 * @param  num_threads   number of threads to be created in the thread_pool
 * @return thread_pool    created thread_pool on success,
 *                       NULL on error
 */
thread_pool thread_pool_init(int num_threads);


/**
 * @brief Add work to the job queue
 *
 * Takes an action and its argument and adds it to the thread_pool's job queue.
 * If you want to add to work a function with more than one arguments then
 * a way to implement this is by passing a pointer to a structure.
 *
 * NOTICE: You have to cast both the function and argument to not get warnings.
 *
 * @example
 *
 *    void print_num(int num){
 *       printf("%d\n", num);
 *    }
 *
 *    int main() {
 *       ..
 *       int a = 10;
 *       thread_pool_add_work(thread_pool, (void*)print_num, (void*)a);
 *       ..
 *    }
 *
 * @param  thread_pool    thread_pool to which the work will be added
 * @param  function_p    pointer to function to add as work
 * @param  arg_p         pointer to an argument
 * @return 0 on success, -1 otherwise.
 */
int thread_pool_add_work(thread_pool thread_pool, void (*function_p)(void *), void *arg_p);


/**
 * @brief Wait for all queued jobs to finish
 *
 * Will wait for all jobs - both queued and currently running to finish.
 * Once the queue is empty and all work has completed, the calling thread
 * (probably the main program) will continue.
 *
 * Smart polling is used in wait. The polling is initially 0 - meaning that
 * there is virtually no polling at all. If after 1 seconds the threads
 * haven't finished, the polling interval starts growing exponentially
 * until it reaches max_secs seconds. Then it jumps down to a maximum polling
 * interval assuming that heavy processing is being used in the thread_pool.
 *
 * @example
 *
 *    ..
 *    thread_pool t_p = thread_pool_init(4);
 *    ..
 *    // Add a bunch of work
 *    ..
 *    thread_pool_wait(t_p);
 *    puts("All added work has finished");
 *    ..
 *
 * @param thread_pool     the thread_pool to wait for
 */
void thread_pool_wait(thread_pool thread_pool);


/**
 * @brief Pauses all threads immediately
 *
 * The threads will be paused no matter if they are idle or working.
 * The threads return to their previous states once thread_pool_resume
 * is called.
 *
 * While the thread is being paused, new work can be added.
 *
 * @example
 *
 *    thread_pool t_p = thread_pool_init(4);
 *    thread_pool_pause(t_p);
 *    ..
 *    // Add a bunch of work
 *    ..
 *    thread_pool_resume(t_p); // Let the threads start their magic
 *
 * @param thread_pool    the thread_pool where the threads should be paused
 */
void thread_pool_pause(thread_pool thread_pool);


/**
 * @brief Resume all threads if they are paused
 *
 * @example
 *    ..
 *    thread_pool_pause(thread_pool);
 *    sleep(10);              // Delay execution 10 seconds
 *    thread_pool_resume(thread_pool);
 *    ..
 *
 * @param thread_pool     the thread_pool where the threads should be resume
 */
void thread_pool_resume(thread_pool thread_pool);


/**
 * @brief Destroy the thread_pool
 *
 * This will wait for the currently active threads to finish and then 'kill'
 * the whole thread_pool to free up memory.
 *
 * @example
 * int main() {
 *    thread_pool t_p1 = thread_pool_init(2);
 *    thread_pool t_p2 = thread_pool_init(2);
 *    ..
 *    thread_pool_destroy(t_p1);
 *    ..
 *    return 0;
 * }
 *
 * @param thread_pool     the thread_pool to destroy
 */
void thread_pool_destroy(thread_pool thread_pool);


/**
 * @brief Show currently working threads
 *
 * Working threads are the threads that are performing work (not idle).
 *
 * @example
 * int main() {
 *    thread_pool t_p1 = thread_pool_init(2);
 *    thread_pool t_p2 = thread_pool_init(2);
 *    ..
 *    printf("Working threads: %d\n", thread_pool_num_threads_working(t_p1));
 *    ..
 *    return 0;
 * }
 *
 * @param thread_pool     the thread_pool of interest
 * @return integer       number of threads working
 */
int thread_pool_num_threads_working(thread_pool thread_pool);


#ifdef __cplusplus
}
#endif

#endif
