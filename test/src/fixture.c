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

#define MIRROR_ID fixture
#include "mirror/mirror.h"



/* test fixtures
 *
 * look ma, no void pointer casting! */



static int int_setup(void) {
    return 4;
}

mirror(fixture(int, i), setup(int_setup)) {
    mirror_eq(i, 4);
}



static int* intp_setup(void) {
    int* x = ghost_alloc(int);
    *x = 4;
    return x;
}

static void intp_teardown(int* i) {
    ghost_free(i);
}

mirror(fixture(int*, i), setup(intp_setup), teardown(intp_teardown)) {
    mirror_eq_i(*i, 4);
}



static FILE* buffer_setup(void) {
    return fopen("/dev/zero", "rb");
}

static void buffer_teardown(FILE* file) {
    fclose(file);
}

/* TODO this should be a suite but suites aren't implemented yet */

mirror(name("file/zero/getc"), fixture(FILE*, file), setup(buffer_setup), teardown(buffer_teardown)) {
    mirror_check(getc(file) == 0);
}

mirror(name("file/zero/fread"), fixture(FILE*, file), setup(buffer_setup), teardown(buffer_teardown)) {
    char b[1];
    mirror_check(fread(b, 1, 1, file) == 1);
    mirror_check(b[0] == 0);
}
