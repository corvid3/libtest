#include "test.h"

const char* passing_test_function(void) {
    return NULL;
}

const char* failing_test_function(void) {
    return "this is a fail";
}

const char* default_fail_function(void) {
    return TESTC_BASIC_ERR;
}

const char* long_function(void) {
    static const struct timespec timer = {.tv_sec = 5};
    nanosleep(&timer, NULL);
    return NULL;
}

static const test_t passing_test = {
    .ptr = passing_test_function,
    .name = "passing test",
    .desc = "",
};

static const test_t failing_test = {
    .ptr = failing_test_function,
    .name = "failing test",
    .desc = "",
};

static const test_t long_test = {
    .ptr = long_function,
    .name = "long test",
    .desc = "",
};

static const test_t default_fail = {
    .ptr = default_fail_function,
    .name = "default fail",
    .desc = "",
};

extern int main(void) {
    const bool pass = execute_test(passing_test);
    const bool fail = execute_test(failing_test);
    execute_test(default_fail);
    execute_test(long_test);

    if (!pass)
        printf("testc failed the test for the passing function\n");
    if (fail)
        printf("testc passed the test for the failing function\n");
}
