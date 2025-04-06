#ifndef STB_JOB_H
#define STB_JOB_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef JOB_IMPLEMENTATION
   #define STB_JOB_IMPLEMENTATION
#endif

#ifndef INITIAL_CAPACITY
    #define INITIAL_CAPACITY 256
#endif
#ifndef ASSERT
    #include <assert.h>
    #define ASSERT assert
#endif
#ifndef CUSTOM_MEMORY
    #include <stdlib.h>
    #define ALLOC malloc
    #define REALLOC realloc
    #define FREE free
#endif

// Thanks to tsoding
#define array_append(arr, item)                                                             \
    do {                                                                                    \
        if ((arr)->count >= (arr)->capacity) {                                              \
            (arr)->capacity = (arr)->capacity == 0 ? INITIAL_CAPACITY : (arr)->capacity*2;  \
            (arr)->items = REALLOC((arr)->items, (arr)->capacity*sizeof(*(arr)->items));    \
            ASSERT((arr)->items != NULL);                                                   \
        }                                                                                   \
                                                                                            \
        (arr)->items[(arr)->count++] = (item);                                              \
    } while (0)

#define array_free(arr) FREE((arr)->items)

typedef void (*job_function_t)(void*, void*);
typedef void (*job_param_t);
typedef struct {
    void *data;
    job_function_t fn;
    bool executed;
} job_t;

typedef struct {
    job_t *items;
    size_t count;
    size_t capacity;

    volatile size_t counter;
    volatile size_t current;
    volatile size_t limit;
} jobs_t;

#ifdef __cplusplus
extern "C" {
#endif

jobs_t *    jobs_init(size_t max_jobs);
void        jobs_free(jobs_t *jm);
void        jobs_add_job(jobs_t *jm, job_function_t job, void *data);
void        jobs_process(jobs_t *jm);
void        job_yield(jobs_t *jm, size_t count);

#ifdef __cplusplus
}
#endif

#endif // STB_JOB_H

#ifdef STB_JOB_IMPLEMENTATION

jobs_t *jobs_init(size_t max_jobs)
{
    jobs_t* jm = (jobs_t*)ALLOC(sizeof(jobs_t));
    ASSERT(jm);

    jm->items = NULL;
    jm->count = 0;
    jm->capacity = 0;
    jm->counter = 0;
    jm->current = 0;
    jm->limit = jm->current + 1;

    return jm;
}

void jobs_free(jobs_t *jm)
{
    ASSERT(jm);

    array_free(jm);

    FREE(jm);
}

void jobs_add_job(jobs_t *jm, job_function_t job, void *data)
{
    ASSERT(jm);

    job_t jobdef = (job_t) {
        .data = data,
        .fn = job,
        .executed = false,
    };

    array_append(jm, jobdef);

    jm->counter += 1;
}

void jobs_process(jobs_t *jm)
{
    ASSERT(jm);

    for (size_t i = jm->current; i < jm->limit && i < jm->counter; ++i) {
        if (jm->items[i].executed) continue;

        jm->items[i].fn(jm, jm->items[i].data);
        jm->items[i].executed = true;

        jm->current -= 1;
    }
}

void job_yield(jobs_t *jm, size_t count)
{
    ASSERT(jm);
    size_t old = jm->current;
    jm->current += 1;

    if (count != 0 && count > 0) {
        jm->limit = jm->current + count;
    } else {
        jm->limit = (jm->current + jm->counter) - 1;
    }

    jobs_process(jm);

    jm->current = old + 1;
    jm->limit = jm->current + 1;
}

#endif // JOB_IMPLEMENTATION
