/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022-2023 Fraser Heavy Software
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef MIRROR_IMPL_RUNNER_COMMON_H
#define MIRROR_IMPL_RUNNER_COMMON_H

/*
 * Common definitions for all runners.
 */

#include "mirror/impl/mirror_impl_declare.h"
#include "mirror/impl/mirror_impl_runner_checks.h"
#include "mirror/impl/mirror_impl_tmmap.h"

/* The largest fixture we'll allocate on the stack */
#if ghost_has(ghost_alloca)
    #ifndef MIRROR_FIXTURE_STACK_THRESHOLD
        #define MIRROR_FIXTURE_STACK_THRESHOLD 64
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

MIRROR_TMMAP_TYPE(mirror_all_tests)
MIRROR_TMMAP_TYPE(mirror_suite_tests)
MIRROR_TMMAP_TYPE(mirror_all_suites)
MIRROR_TMMAP_TYPE(mirror_suite_suites)

typedef int ghost_impl_chibicc_unused4; /* chibicc workaround: https://github.com/rui314/chibicc/issues/99 */

#define mirror_tests_key(test) test->name
MIRROR_TMMAP_STATIC(mirror_all_tests, const char*, mirror_test_t, all_tests, mirror_tests_key, ghost_strcmp)
MIRROR_TMMAP_STATIC(mirror_suite_tests, const char*, mirror_test_t, suite_tests, mirror_tests_key, ghost_strcmp)

typedef int ghost_impl_chibicc_unused3; /* chibicc workaround: https://github.com/rui314/chibicc/issues/99 */

#define mirror_suites_key(suite) suite->name
MIRROR_TMMAP_STATIC(mirror_all_suites, const char*, mirror_suite_t, all_suites, mirror_suites_key, ghost_strcmp)
MIRROR_TMMAP_STATIC(mirror_suite_suites, const char*, mirror_suite_t, suite_suites, mirror_suites_key, ghost_strcmp)

/* Global maps */
static mirror_all_tests_t* mirror_all_tests(void) {
    static mirror_all_tests_t tests;
    return &tests;
}
#if 0
static mirror_suite_tests_t* mirror_suite_tests(void) {
    static mirror_suite_tests_t tests;
    return &tests;
}
static mirror_all_suites_t* mirror_all_suites(void) {
    static mirror_all_suites_t suites;
    return &suites;
}
static mirror_suite_suites_t* mirror_suite_suites(void) {
    static mirror_suite_suites_t suites;
    return &suites;
}
#endif

void mirror_register_test(mirror_test_t* test) {
    mirror_all_tests_insert_last(mirror_all_tests(), test);
}

static void mirror_init(void) {

    #if !ghost_has(ghost_static_init)
    mirror_register_all();
    #endif

    #if 0
    /* Link all test cases with their suites. */
    for (mirror_test_t* test = mirror_test_first;
            test != ghost_null; test = test->next_globally) {

        /* Find the test suite for this test case */
        mirror_suite_t* suite;
        for (suite = mirror_suite_first;; suite = suite->next_globally) {
            if (suite == ghost_null) {

                /* No suite was found with a matching name. Create one. */
                #if ghost_has(ghost_alloc_zero)
                    printf("Allocated test suite for %s\n", test->name);
                    suite = ghost_alloc_zero(mirror_suite_t);
                    suite->name = test->suite_name;
                    suite->allocated = ghost_true;
                    suite->next_globally = mirror_suite_first;
                    mirror_suite_first = suite;
                    suite->first_test = test;
                    test->suite = suite;
                    break;
                #else
                    fprintf(stderr, "ERROR: No ghost test suite found for %s/%s "
                            "(and no malloc() available to create it.)\n",
                            test->suite_name, test->name);
                    ghost_abort();
                #endif
            }

            if (0 == ghost_strcmp(test->suite_name, suite->name)) {
                test->suite = suite;
                test->next_in_suite = suite->first_test;
                suite->first_test = test;
                break;
            }
        }
    }
    #endif

    /* Link all test suites with their parents.
     * TODO */

}

static void mirror_teardown(void) {

    #if 0
    /* Free all allocated suites. */
    for (mirror_suite_t* suite = mirror_suite_first; suite != ghost_null;) {
        mirror_suite_t* next = suite->next_globally;
        if (suite->allocated)
            ghost_free(suite);
        suite = next;
    }
    #endif

}

/* We noinline this explicitly because we use alloca() for the fixture. */
ghost_noinline
static void mirror_run(mirror_test_t* test) {
    /*
    printf("%s() %i\n",__func__,__LINE__);
    printf("%p\n",(void*)test);
    printf("%s\n",test->id);
    printf("%p\n",(void*)test->suite);
    */
    mirror_suite_t* suite = test->suite;
    /*
    printf("%s() %i\n",__func__,__LINE__);
    */
    /*printf("Running %s\n", test->name); */
    void* fixture = ghost_null;

    if (test->fixture_size != 0) {
        #if ghost_has_ghost_alloca
        if (test->fixture_size <= MIRROR_FIXTURE_STACK_THRESHOLD) {
            fixture = ghost_alloca(test->fixture_size);
            ghost_bzero(fixture, test->fixture_size);
        } else
        #endif
        {
            #if ghost_has_ghost_calloc
                fixture = ghost_calloc(test->fixture_size, 1);
            #else
                fprintf(stderr, "Cannot allocate fixture of size %" GHOST_PRIuZ
                        " for %s without malloc().\n"
                        "Consider raising MIRROR_FIXTURE_STACK_THRESHOLD.\n",
                        test->fixture_size, test->name);
                ghost_abort();
            #endif
        }
    }

    /* TODO */
    /*int x=5;
    void* param = &x;*/

    if (test && test->fixture_setup)
        test->fixture_setup(fixture);
    test->fn(fixture, ghost_null/*TODO param*/);
    if (test && test->fixture_teardown)
        test->fixture_teardown(fixture);

    if (suite && suite->fixture_size != 0) {
        #if ghost_has(ghost_alloca)
        if (suite->fixture_size <= MIRROR_FIXTURE_STACK_THRESHOLD) {
            /* nothing */
        } else
        #endif
        {
            #if ghost_has(ghost_free)
            ghost_free(fixture);
            #endif
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif
