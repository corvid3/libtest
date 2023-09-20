
#pragma once

/*
single headerfile testing suite
prints output to STDOUT, relies on a VT105 compatable terminal

include this /only once/, then pass test functions
to the "execute tests" function

credits: korvo
    (korvonesto.com)
*/

#include <bits/types/struct_timespec.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

// c macros are brilliant
#define TESTC_BASIC_ERR__(v) #v
#define TESTC_BASIC_ERR_(v) TESTC_BASIC_ERR__(v)
#define TESTC_BASIC_ERR "Err @ " TESTC_BASIC_ERR_(__LINE__)

typedef struct {
    const char* name;
    const char* desc;
    const char* (*ptr)(void);
} test_t;

typedef struct {
    const char* (*test_fn)(void);
    bool finished;
    double time_out;
    const char* fail_out;
} wrapper_args;

static void* test_wrapper(void* _args) {
    wrapper_args* args = (wrapper_args*)_args;

    time_t begin_time, end_time;
    time(&begin_time);

    const char* val = args->test_fn();

    time(&end_time);

    double time_exerted = difftime(end_time, begin_time);

    args->finished = true;
    args->fail_out = val;
    args->time_out = time_exerted;

    return NULL;
}

// returns true on pass
static bool execute_test(const test_t test) {
    wrapper_args args = {
        .test_fn = test.ptr,
        .finished = false,
        .time_out = 0.f,
        .fail_out = NULL,
    };

    pthread_t test_thread;
    pthread_create(&test_thread, NULL, test_wrapper, &args);

    printf("\033[1K\033[0G");
    printf("%s: ", test.name);
    fflush(stdout);

    int msec = 0;
    int ticks = 0;
    while (!args.finished) {
        static const struct timespec wait = {.tv_sec = 0, .tv_nsec = 1000000};
        nanosleep(&wait, NULL);
        msec += 1;
        if (msec > 1000) {
            msec %= 1000;
            ticks += 1;
            ticks %= 5;
            printf("\033[1k\033[0G%s: ", test.name);
            for (int i = 0; i < ticks; i++) {
                printf(".");
            }
            fflush(stdout);
        }
    }

    pthread_join(test_thread, NULL);

    if (args.fail_out == NULL)
        printf("\033[1K\033[0G%s: SUCCESS!", test.name);
    else
        printf("\033[1K\033[0G%s: Error [\"%s\"]", test.name, args.fail_out);

    printf("\n\tFinished in: %.2fs\n", args.time_out);

    fflush(stdout);

    return args.fail_out ? false : true;
}
