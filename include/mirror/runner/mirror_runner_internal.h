/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Fraser Heavy Software
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

#ifndef MIRROR_RUNNER_INTERNAL_H
#define MIRROR_RUNNER_INTERNAL_H

/**
 * This is Mirror's internal test runner.
 */

/*TODO*/
#include "ghost/header/c/ghost_stdio_h.h"
#include "ghost/header/c/ghost_stdlib_h.h"

#include "mirror/impl/mirror_impl_runner_common.h"

#if 0
static void mirror_suite_run(mirror_suite_t* suite) {

    /* Run tests in suite */
    for (mirror_test_t* test = suite->first_test;
            test != ghost_null; test = test->next_in_suite) {
        mirror_run(test);
    }

    /* Recurse into child suites */
    for (mirror_suite_t* child = suite->first_suite;
            child != ghost_null; child = child->next_in_parent) {
        mirror_suite_run(child);
    }

}
#endif

int main(void) {
    mirror_test_t* test;

    mirror_init();

(void)&mirror_run;
    #if 0
    for (mirror_suite_t* suite = mirror_suite_first;
            suite != ghost_null; suite = suite->next_globally) {

        /* Skip any suites that have parents. We'll run them as part of their
         * parent suites. */
        if (suite->parent != ghost_null)
            continue;

        /* Run the suite. */
        mirror_suite_run(suite);
    }
    #endif

    for (test = mirror_all_tests_first(mirror_all_tests());
            test;
            test = mirror_all_tests_next(mirror_all_tests(), test))
    {
        mirror_run(test);
    }

    mirror_teardown();

    printf("All %" GHOST_PRIdZ " tests pass.\n", mirror_all_tests_count(mirror_all_tests()));

        #ifdef __PCC__
        _Exit(EXIT_SUCCESS);
        #endif

    return EXIT_SUCCESS;
}

#endif
