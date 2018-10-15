#define _POSIX_C_SOURCE 200809L

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#if defined(__linux__)
#include <sys/prctl.h>
#endif

#include "mcf_thread_pool.h"

#ifdef MCF_THREAD_POOL_DEBUG
#define MCF_THREAD_POOL_DEBUG 1
#else
#define MCF_THREAD_POOL_DEBUG 0
#endif

#if !defined(DISABLE_PRINT) || defined(MCF_THREAD_POOL_DEBUG)
#define err(str) fprintf(stderr, str)
#else
#define err(str)
#endif

static volatile int threads_keepalive;
static volatile int threads_on_hold;



/* ========================== STRUCTURES ============================ */


/* Binary semaphore */
typedef struct mcf_binary_semaphore {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int v;
} mcf_binary_semaphore;


/* Job */
typedef struct job {
    struct job *prev;                   /* pointer to previous job   */
    void (*function)(void *arg);       /* function pointer          */
    void *arg;                          /* function's argument       */
} job;


/* Job queue */
typedef struct job_queue {
    pthread_mutex_t rwmutex;             /* used for queue r/w access */
    job *front;                         /* pointer to front of queue */
    job *rear;                          /* pointer to rear  of queue */
    mcf_binary_semaphore *has_jobs;      /* flag as binary semaphore  */
    int length;                        /* number of jobs in queue   */
} job_queue;


/* Thread */
typedef struct thread {
    int id;                                        /* friendly id               */
    pthread_t pthread;                                /* pointer to actual thread  */
    struct mcf_thread_pool_ *thread_pool_p;                /* access to thread_pool          */
} thread;


/* Thread_pool */
typedef struct mcf_thread_pool_ {
    thread **threads;                        /* pointer to threads        */
    volatile int num_threads_alive;            /* threads currently alive   */
    volatile int num_threads_working;            /* threads currently working */
    pthread_mutex_t thread_count_lock;        /* used for thread count etc */
    pthread_cond_t threads_all_idle;            /* signal to thread_pool_wait     */
    job_queue job_queue;                        /* job queue                 */
} mcf_thread_pool_;


/* ========================== PROTOTYPES ============================ */


static int thread_init(mcf_thread_pool_ *thread_pool_p, struct thread **thread_p, int id);

static void *thread_do(struct thread *thread_p);

static void thread_hold(int sig_id);

static void thread_destroy(struct thread *thread_p);

static int job_queue_init(job_queue *job_queue_p);

static void job_queue_clear(job_queue *job_queue_p);

static void job_queue_push(job_queue *job_queue_p, struct job *new_job_p);

static struct job *job_queue_pull(job_queue *job_queue_p);

static void job_queue_destroy(job_queue *job_queue_p);

static void binary_semaphore_init(struct mcf_binary_semaphore *binary_semaphore_p, int value);

static void binary_semaphore_reset(struct mcf_binary_semaphore *binary_semaphore_p);

static void binary_semaphore_post(struct mcf_binary_semaphore *binary_semaphore_p);

static void binary_semaphore_post_all(struct mcf_binary_semaphore *binary_semaphore_p);

static void binary_semaphore_wait(struct mcf_binary_semaphore *binary_semaphore_p);


/* ========================== THREAD_POOL ============================ */


/* Initialise thread pool */
struct mcf_thread_pool_ *thread_pool_init(int num_threads) {

    threads_on_hold = 0;
    threads_keepalive = 1;

    if (num_threads < 0) {
        num_threads = 0;
    }

    /* Make new thread pool */
    mcf_thread_pool_ *thread_pool_p;
    thread_pool_p = (struct mcf_thread_pool_ *) malloc(sizeof(struct mcf_thread_pool_));
    if (thread_pool_p == NULL) {
        err("thread_pool_init(): Could not allocate memory for thread pool\n");
        return NULL;
    }
    thread_pool_p->num_threads_alive = 0;
    thread_pool_p->num_threads_working = 0;

    /* Initialise the job queue */
    if (job_queue_init(&thread_pool_p->job_queue) == -1) {
        err("thread_pool_init(): Could not allocate memory for job queue\n");
        free(thread_pool_p);
        return NULL;
    }

    /* Make threads in pool */
    thread_pool_p->threads = (struct thread **) malloc(num_threads * sizeof(struct thread *));
    if (thread_pool_p->threads == NULL) {
        err("thread_pool_init(): Could not allocate memory for threads\n");
        job_queue_destroy(&thread_pool_p->job_queue);
        free(thread_pool_p);
        return NULL;
    }

    pthread_mutex_init(&(thread_pool_p->thread_count_lock), NULL);
    pthread_cond_init(&thread_pool_p->threads_all_idle, NULL);

    /* Thread init */
    int n;
    for (n = 0; n < num_threads; n++) {
        thread_init(thread_pool_p, &thread_pool_p->threads[n], n);
#if MCF_THREAD_POOL_DEBUG
        printf("MCF_THREAD_POOL_DEBUG: Created thread %d in pool \n", n);
#endif
    }

    /* Wait for threads to initialize */
    while (thread_pool_p->num_threads_alive != num_threads) {}

    return thread_pool_p;
}


/* Add work to the thread pool */
int thread_pool_add_work(thread_pool thread_pool_p, void (*function_p)(void *), void *arg_p) {
    job *new_job;

    new_job = (struct job *) malloc(sizeof(struct job));
    if (new_job == NULL) {
        err("thread_pool_add_work(): Could not allocate memory for new job\n");
        return -1;
    }

    /* add function and argument */
    new_job->function = function_p;
    new_job->arg = arg_p;

    /* add job to queue */
    job_queue_push(&thread_pool_p->job_queue, new_job);

    return 0;
}


/* Wait until all jobs have finished */
void thread_pool_wait(mcf_thread_pool_ *thread_pool_p) {
    pthread_mutex_lock(&thread_pool_p->thread_count_lock);
    while (thread_pool_p->job_queue.length || thread_pool_p->num_threads_working) {
        pthread_cond_wait(&thread_pool_p->threads_all_idle, &thread_pool_p->thread_count_lock);
    }
    pthread_mutex_unlock(&thread_pool_p->thread_count_lock);
}


/* Destroy the thread_pool */
void thread_pool_destroy(mcf_thread_pool_ *thread_pool_p) {
    /* No need to destroy if it's NULL */
    if (thread_pool_p == NULL) return;

    volatile int threads_total = thread_pool_p->num_threads_alive;

    /* End each thread 's infinite loop */
    threads_keepalive = 0;

    /* Give one second to kill idle threads */
    double TIMEOUT = 1.0;
    time_t start, end;
    double tpassed = 0.0;
    time(&start);
    while (tpassed < TIMEOUT && thread_pool_p->num_threads_alive) {
        binary_semaphore_post_all(thread_pool_p->job_queue.has_jobs);
        time(&end);
        tpassed = difftime(end, start);
    }

    /* Poll remaining threads */
    while (thread_pool_p->num_threads_alive) {
        binary_semaphore_post_all(thread_pool_p->job_queue.has_jobs);
        sleep(1);
    }

    /* Job queue cleanup */
    job_queue_destroy(&thread_pool_p->job_queue);
    /* Dealloc */
    int n;
    for (n = 0; n < threads_total; n++) {
        thread_destroy(thread_pool_p->threads[n]);
    }
    free(thread_pool_p->threads);
    free(thread_pool_p);
}


/* Pause all threads in thread_pool */
void thread_pool_pause(mcf_thread_pool_ *thread_pool_p) {
    int n;
    for (n = 0; n < thread_pool_p->num_threads_alive; n++) {
        pthread_kill(thread_pool_p->threads[n]->pthread, SIGUSR1);
    }
}


/* Resume all threads in thread_pool */
void thread_pool_resume(mcf_thread_pool_ *thread_pool_p) {
    // resuming a single thread_pool hasn't been
    // implemented yet, meanwhile this supresses
    // the warnings
    (void) thread_pool_p;

    threads_on_hold = 0;
}


int thread_pool_num_threads_working(mcf_thread_pool_ *thread_pool_p) {
    return thread_pool_p->num_threads_working;
}





/* ============================ THREAD ============================== */


/* Initialize a thread in the thread pool
 *
 * @param thread        address to the pointer of the thread to be created
 * @param id            id to be given to the thread
 * @return 0 on success, -1 otherwise.
 */
static int thread_init(mcf_thread_pool_ *thread_pool_p, struct thread **thread_p, int id) {

    *thread_p = (struct thread *) malloc(sizeof(struct thread));
    if (thread_p == NULL) {
        err("thread_init(): Could not allocate memory for thread\n");
        return -1;
    }

    (*thread_p)->thread_pool_p = thread_pool_p;
    (*thread_p)->id = id;

    pthread_create(&(*thread_p)->pthread, NULL, (void *) thread_do, (*thread_p));
    pthread_detach((*thread_p)->pthread);
    return 0;
}


/* Sets the calling thread on hold */
static void thread_hold(int sig_id) {
    (void) sig_id;
    threads_on_hold = 1;
    while (threads_on_hold) {
        sleep(1);
    }
}


/* What each thread is doing
*
* In principle this is an endless loop. The only time this loop gets interuppted is once
* thread_pool_destroy() is invoked or the program exits.
*
* @param  thread        thread that will run this function
* @return nothing
*/
static void *thread_do(struct thread *thread_p) {

    /* Set thread name for profiling and debuging */
    char thread_name[128] = {0};
    sprintf(thread_name, "top.limengyu.mcf_thread_pool.thread_%d", thread_p->id);

#if defined(__linux__)
    /* Use prctl instead to prevent using _GNU_SOURCE flag and implicit declaration */
    prctl(PR_SET_NAME, thread_name);
#elif defined(__APPLE__) && defined(__MACH__)
    pthread_setname_np(thread_name);
#else
    err("thread_do(): pthread_setname_np is not supported on this system");
#endif

    /* Assure all threads have been created before starting serving */
    mcf_thread_pool_ *thread_pool_p = thread_p->thread_pool_p;

    /* Register signal handler */
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = thread_hold;
    if (sigaction(SIGUSR1, &act, NULL) == -1) {
        err("thread_do(): cannot handle SIGUSR1");
    }

    /* Mark thread as alive (initialized) */
    pthread_mutex_lock(&thread_pool_p->thread_count_lock);
    thread_pool_p->num_threads_alive += 1;
    pthread_mutex_unlock(&thread_pool_p->thread_count_lock);

    while (threads_keepalive) {

        binary_semaphore_wait(thread_pool_p->job_queue.has_jobs);

        if (threads_keepalive) {

            pthread_mutex_lock(&thread_pool_p->thread_count_lock);
            thread_pool_p->num_threads_working++;
            pthread_mutex_unlock(&thread_pool_p->thread_count_lock);

            /* Read job from queue and execute it */
            void (*func_buff)(void *);
            void *arg_buff;
            job *job_p = job_queue_pull(&thread_pool_p->job_queue);
            if (job_p) {
                func_buff = job_p->function;
                arg_buff = job_p->arg;
                func_buff(arg_buff);
                free(job_p);
            }

            pthread_mutex_lock(&thread_pool_p->thread_count_lock);
            thread_pool_p->num_threads_working--;
            if (!thread_pool_p->num_threads_working) {
                pthread_cond_signal(&thread_pool_p->threads_all_idle);
            }
            pthread_mutex_unlock(&thread_pool_p->thread_count_lock);

        }
    }
    pthread_mutex_lock(&thread_pool_p->thread_count_lock);
    thread_pool_p->num_threads_alive--;
    pthread_mutex_unlock(&thread_pool_p->thread_count_lock);

    return NULL;
}


/* Frees a thread  */
static void thread_destroy(thread *thread_p) {
    free(thread_p);
}





/* ============================ JOB QUEUE =========================== */


/* Initialize queue */
static int job_queue_init(job_queue *job_queue_p) {
    job_queue_p->length = 0;
    job_queue_p->front = NULL;
    job_queue_p->rear = NULL;

    job_queue_p->has_jobs = (struct mcf_binary_semaphore *) malloc(sizeof(struct mcf_binary_semaphore));
    if (job_queue_p->has_jobs == NULL) {
        return -1;
    }

    pthread_mutex_init(&(job_queue_p->rwmutex), NULL);
    binary_semaphore_init(job_queue_p->has_jobs, 0);

    return 0;
}


/* Clear the queue */
static void job_queue_clear(job_queue *job_queue_p) {

    while (job_queue_p->length) {
        free(job_queue_pull(job_queue_p));
    }

    job_queue_p->front = NULL;
    job_queue_p->rear = NULL;
    binary_semaphore_reset(job_queue_p->has_jobs);
    job_queue_p->length = 0;

}


/* Add (allocated) job to queue
 */
static void job_queue_push(job_queue *job_queue_p, struct job *new_job_p) {

    pthread_mutex_lock(&job_queue_p->rwmutex);
    new_job_p->prev = NULL;

    switch (job_queue_p->length) {

        case 0:  /* if no jobs in queue */
            job_queue_p->front = new_job_p;
            job_queue_p->rear = new_job_p;
            break;

        default: /* if jobs in queue */
            job_queue_p->rear->prev = new_job_p;
            job_queue_p->rear = new_job_p;

    }
    job_queue_p->length++;

    binary_semaphore_post(job_queue_p->has_jobs);
    pthread_mutex_unlock(&job_queue_p->rwmutex);
}


/* Get first job from queue(removes it from queue)
<<<<<<< HEAD
 *
 * Notice: Caller MUST hold a mutex
=======
>>>>>>> da2c0fe45e43ce0937f272c8cd2704bdc0afb490
 */
static struct job *job_queue_pull(job_queue *job_queue_p) {

    pthread_mutex_lock(&job_queue_p->rwmutex);
    job *job_p = job_queue_p->front;

    switch (job_queue_p->length) {

        case 0:  /* if no jobs in queue */
            break;

        case 1:  /* if one job in queue */
            job_queue_p->front = NULL;
            job_queue_p->rear = NULL;
            job_queue_p->length = 0;
            break;

        default: /* if >1 jobs in queue */
            job_queue_p->front = job_p->prev;
            job_queue_p->length--;
            /* more than one job in queue -> post it */
            binary_semaphore_post(job_queue_p->has_jobs);

    }

    pthread_mutex_unlock(&job_queue_p->rwmutex);
    return job_p;
}


/* Free all queue resources back to the system */
static void job_queue_destroy(job_queue *job_queue_p) {
    job_queue_clear(job_queue_p);
    free(job_queue_p->has_jobs);
}





/* ======================== SYNCHRONISATION ========================= */


/* Init semaphore to 1 or 0 */
static void binary_semaphore_init(mcf_binary_semaphore *binary_semaphore_p, int value) {
    if (value < 0 || value > 1) {
        err("binary_semaphore_init(): Binary semaphore can take only values 1 or 0");
        exit(1);
    }
    pthread_mutex_init(&(binary_semaphore_p->mutex), NULL);
    pthread_cond_init(&(binary_semaphore_p->cond), NULL);
    binary_semaphore_p->v = value;
}


/* Reset semaphore to 0 */
static void binary_semaphore_reset(mcf_binary_semaphore *binary_semaphore_p) {
    binary_semaphore_init(binary_semaphore_p, 0);
}


/* Post to at least one thread */
static void binary_semaphore_post(mcf_binary_semaphore *binary_semaphore_p) {
    pthread_mutex_lock(&binary_semaphore_p->mutex);
    binary_semaphore_p->v = 1;
    pthread_cond_signal(&binary_semaphore_p->cond);
    pthread_mutex_unlock(&binary_semaphore_p->mutex);
}


/* Post to all threads */
static void binary_semaphore_post_all(mcf_binary_semaphore *binary_semaphore_p) {
    pthread_mutex_lock(&binary_semaphore_p->mutex);
    binary_semaphore_p->v = 1;
    pthread_cond_broadcast(&binary_semaphore_p->cond);
    pthread_mutex_unlock(&binary_semaphore_p->mutex);
}


/* Wait on semaphore until semaphore has value 0 */
static void binary_semaphore_wait(mcf_binary_semaphore *binary_semaphore_p) {
    pthread_mutex_lock(&binary_semaphore_p->mutex);
    while (binary_semaphore_p->v != 1) {
        pthread_cond_wait(&binary_semaphore_p->cond, &binary_semaphore_p->mutex);
    }
    binary_semaphore_p->v = 0;
    pthread_mutex_unlock(&binary_semaphore_p->mutex);
}
