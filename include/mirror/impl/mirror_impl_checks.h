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

#ifndef MIRROR_IMPL_CHECKS_H
#define MIRROR_IMPL_CHECKS_H

#include "mirror/impl/mirror_impl_ghost.h"

#ifdef __cplusplus
extern "C" {
#endif

/* TODO passing these through expands macros before stringification. in all
 * cases we need to stringify arguments before forwarding it off to some check
 * function. */

/**
 * @def mirror_check(x) if (!x) fail()
 *
 * Checks that the given expression is true.
 */

/* TODO pass a message, stringify the arg */
#define mirror_check(x) ghost_ensure(x, "mirror check failed") /* TODO consider calling this mirror_true() */

/**
 * @def mirror_eq(x, y) if (x != y) fail()
 *
 * Checks that the given values match by comparing with ==.
 */

#define mirror_equal mirror_eq /* TODO get rid of this */

/* TODO GHOST_INSERT_COMMA isn't working
 *#define mirror_error(...) ghost_ensure(0 GHOST_INSERT_COMMA(__VA_ARGS__)) */
#define mirror_error() ghost_ensure(0, "mirror check failed")

/* TODO */
#define mirror_eq(x, y) mirror_check((x) == (y))
#define mirror_ne(x, y) mirror_check((x) != (y))
#define mirror_lt(x, y) mirror_check((x) < (y))
#define mirror_le(x, y) mirror_check((x) <= (y))
#define mirror_gt(x, y) mirror_check((x) > (y))
#define mirror_ge(x, y) mirror_check((x) >= (y))

#define mirror_fail() mirror_check(0)

#define mirror_nn(x) mirror_check(ghost_null != (x))



ghost_maybe_unused
static void mirror_handle_failure(const char* file, int line, const char* message) {
    printf("%s:%i %s", file, line, message);
    ghost_fatal("");
}


typedef enum mirror_op_t {
    mirror_op_eq,  /* equal */
    mirror_op_ne,  /* not equal */
    mirror_op_lt,  /* less than */
    mirror_op_le,  /* less than or equal to */
    mirror_op_gt,  /* greater than */
    mirror_op_ge,  /* greater than or equal to */
    mirror_op_eqb, /* equal bitwise, for floats */
    mirror_op_eqe  /* equal within acceptable error, for floats */
} mirror_op_t;

void mirror_fail_cmp(const char* file, int line, mirror_op_t op,
        const char* fx, const char* sx, const char* fy, const char* sy);



/* chars */

/* char */
#define mirror_eq_c(x, y) mirror_impl_cmp_c(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_c(x, y) mirror_impl_cmp_c(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_c(x, y) mirror_impl_cmp_c(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_c(x, y) mirror_impl_cmp_c(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_c(x, y) mirror_impl_cmp_c(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_c(x, y) mirror_impl_cmp_c(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_c(const char* file, int line, mirror_op_t op,
        char x, const char* sx,
        char y, const char* sy);

/* signed char */
#define mirror_eq_sc(x, y) mirror_impl_cmp_sc(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_sc(x, y) mirror_impl_cmp_sc(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_sc(x, y) mirror_impl_cmp_sc(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_sc(x, y) mirror_impl_cmp_sc(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_sc(x, y) mirror_impl_cmp_sc(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_sc(x, y) mirror_impl_cmp_sc(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_sc(const char* file, int line, mirror_op_t op,
        signed char x, const char* sx,
        signed char y, const char* sy);

/* unsigned char */
#define mirror_eq_uc(x, y) mirror_impl_cmp_uc(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_uc(x, y) mirror_impl_cmp_uc(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_uc(x, y) mirror_impl_cmp_uc(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_uc(x, y) mirror_impl_cmp_uc(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_uc(x, y) mirror_impl_cmp_uc(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_uc(x, y) mirror_impl_cmp_uc(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_uc(const char* file, int line, mirror_op_t op,
        unsigned char x, const char* sx,
        unsigned char y, const char* sy);

/* char8_t */
#define mirror_eq_c8(x, y) mirror_impl_cmp_uc(__FILE__, __LINE__, mirror_op_eq, ghost_static_cast(unsigned char, x), #x, ghost_static_cast(unsigned char, y), #y)
#define mirror_ne_c8(x, y) mirror_impl_cmp_uc(__FILE__, __LINE__, mirror_op_ne, ghost_static_cast(unsigned char, x), #x, ghost_static_cast(unsigned char, y), #y)
#define mirror_lt_c8(x, y) mirror_impl_cmp_uc(__FILE__, __LINE__, mirror_op_lt, ghost_static_cast(unsigned char, x), #x, ghost_static_cast(unsigned char, y), #y)
#define mirror_le_c8(x, y) mirror_impl_cmp_uc(__FILE__, __LINE__, mirror_op_le, ghost_static_cast(unsigned char, x), #x, ghost_static_cast(unsigned char, y), #y)
#define mirror_gt_c8(x, y) mirror_impl_cmp_uc(__FILE__, __LINE__, mirror_op_gt, ghost_static_cast(unsigned char, x), #x, ghost_static_cast(unsigned char, y), #y)
#define mirror_ge_c8(x, y) mirror_impl_cmp_uc(__FILE__, __LINE__, mirror_op_ge, ghost_static_cast(unsigned char, x), #x, ghost_static_cast(unsigned char, y), #y)
/* char8_t is always forwarded to unsigned char regardless of the platform. */

/* char16_t */
#define mirror_eq_c16(x, y) mirror_impl_cmp_c16(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_c16(x, y) mirror_impl_cmp_c16(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_c16(x, y) mirror_impl_cmp_c16(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_c16(x, y) mirror_impl_cmp_c16(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_c16(x, y) mirror_impl_cmp_c16(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_c16(x, y) mirror_impl_cmp_c16(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_c16(const char* file, int line, mirror_op_t op,
        ghost_char16_t x, const char* sx,
        ghost_char16_t y, const char* sy);

/* char32_t */
#define mirror_eq_c32(x, y) mirror_impl_cmp_c32(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_c32(x, y) mirror_impl_cmp_c32(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_c32(x, y) mirror_impl_cmp_c32(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_c32(x, y) mirror_impl_cmp_c32(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_c32(x, y) mirror_impl_cmp_c32(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_c32(x, y) mirror_impl_cmp_c32(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_c32(const char* file, int line, mirror_op_t op,
        ghost_char32_t x, const char* sx,
        ghost_char32_t y, const char* sy);



/* floats */

/* float */
#if ghost_has(ghost_float)
    #define mirror_eq_f(x, y) mirror_impl_cmp_f(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
    #define mirror_ne_f(x, y) mirror_impl_cmp_f(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
    #define mirror_lt_f(x, y) mirror_impl_cmp_f(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
    #define mirror_le_f(x, y) mirror_impl_cmp_f(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
    #define mirror_gt_f(x, y) mirror_impl_cmp_f(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
    #define mirror_ge_f(x, y) mirror_impl_cmp_f(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
    void mirror_impl_cmp_f(const char* file, int line, mirror_op_t op,
            float x, const char* sx,
            float y, const char* sy);

    #define mirror_eqe_f(x, y, error) mirror_impl_eqe_f(__FILE__, __LINE__, x, #x, y, #y, error, #error)
    void mirror_impl_eqe_f(const char* file, int line,
            float x, const char* sx,
            float y, const char* sy,
            float error, const char* serror);
#endif

/* double */
#if ghost_has(ghost_double)
    #define mirror_eq_d(x, y) mirror_impl_cmp_d(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
    #define mirror_ne_d(x, y) mirror_impl_cmp_d(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
    #define mirror_lt_d(x, y) mirror_impl_cmp_d(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
    #define mirror_le_d(x, y) mirror_impl_cmp_d(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
    #define mirror_gt_d(x, y) mirror_impl_cmp_d(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
    #define mirror_ge_d(x, y) mirror_impl_cmp_d(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
    void mirror_impl_cmp_d(const char* file, int line, mirror_op_t op,
            double x, const char* sx,
            double y, const char* sy);

    #define mirror_eqe_d(x, y, error) mirror_impl_eqe_d(__FILE__, __LINE__, x, #x, y, #y, error, #error)
    void mirror_impl_eqe_d(const char* file, int line,
            double x, const char* sx,
            double y, const char* sy,
            double error, const char* serror);
#endif

/* long double */
#if ghost_has(ghost_ldouble)
    #define mirror_eq_ld(x, y) mirror_impl_cmp_ld(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
    #define mirror_ne_ld(x, y) mirror_impl_cmp_ld(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
    #define mirror_lt_ld(x, y) mirror_impl_cmp_ld(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
    #define mirror_le_ld(x, y) mirror_impl_cmp_ld(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
    #define mirror_gt_ld(x, y) mirror_impl_cmp_ld(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
    #define mirror_ge_ld(x, y) mirror_impl_cmp_ld(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
    void mirror_impl_cmp_ld(const char* file, int line, mirror_op_t op,
            long double x, const char* sx,
            long double y, const char* sy);
#endif

/* float32_t */
#if ghost_has(ghost_float32_t)
    #define mirror_eq_f32(x, y) mirror_impl_cmp_f32(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
    #define mirror_ne_f32(x, y) mirror_impl_cmp_f32(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
    #define mirror_lt_f32(x, y) mirror_impl_cmp_f32(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
    #define mirror_le_f32(x, y) mirror_impl_cmp_f32(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
    #define mirror_gt_f32(x, y) mirror_impl_cmp_f32(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
    #define mirror_ge_f32(x, y) mirror_impl_cmp_f32(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
    void mirror_impl_cmp_f32(const char* file, int line, mirror_op_t op,
            ghost_float32_t x, const char* sx,
            ghost_float32_t y, const char* sy);
#endif

/* float64_t */
#if ghost_has(ghost_float64_t)
    #define mirror_eq_f64(x, y) mirror_impl_cmp_f64(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
    #define mirror_ne_f64(x, y) mirror_impl_cmp_f64(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
    #define mirror_lt_f64(x, y) mirror_impl_cmp_f64(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
    #define mirror_le_f64(x, y) mirror_impl_cmp_f64(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
    #define mirror_gt_f64(x, y) mirror_impl_cmp_f64(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
    #define mirror_ge_f64(x, y) mirror_impl_cmp_f64(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
    void mirror_impl_cmp_f64(const char* file, int line, mirror_op_t op,
            ghost_float64_t x, const char* sx,
            ghost_float64_t y, const char* sy);
#endif



/* other basic types: short, int, long, long long */

/* short */
#define mirror_eq_h(x, y) mirror_impl_cmp_h(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_h(x, y) mirror_impl_cmp_h(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_h(x, y) mirror_impl_cmp_h(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_h(x, y) mirror_impl_cmp_h(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_h(x, y) mirror_impl_cmp_h(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_h(x, y) mirror_impl_cmp_h(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_h(const char* file, int line, mirror_op_t op,
        short x, const char* sx,
        short y, const char* sy);

/* unsigned short */
#define mirror_eq_uh(x, y) mirror_impl_cmp_uh(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_uh(x, y) mirror_impl_cmp_uh(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_uh(x, y) mirror_impl_cmp_uh(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_uh(x, y) mirror_impl_cmp_uh(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_uh(x, y) mirror_impl_cmp_uh(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_uh(x, y) mirror_impl_cmp_uh(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_uh(const char* file, int line, mirror_op_t op,
        unsigned short x, const char* sx,
        unsigned short y, const char* sy);

/* int */
#define mirror_eq_i(x, y) mirror_impl_cmp_i(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_i(x, y) mirror_impl_cmp_i(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_i(x, y) mirror_impl_cmp_i(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_i(x, y) mirror_impl_cmp_i(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_i(x, y) mirror_impl_cmp_i(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_i(x, y) mirror_impl_cmp_i(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_i(const char* file, int line, mirror_op_t op,
        int x, const char* sx,
        int y, const char* sy);

/* unsigned (int) */
#define mirror_eq_u(x, y) mirror_impl_cmp_u(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_u(x, y) mirror_impl_cmp_u(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_u(x, y) mirror_impl_cmp_u(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_u(x, y) mirror_impl_cmp_u(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_u(x, y) mirror_impl_cmp_u(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_u(x, y) mirror_impl_cmp_u(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_u(const char* file, int line, mirror_op_t op,
        unsigned x, const char* sx,
        unsigned y, const char* sy);

/* long */
#define mirror_eq_l(x, y) mirror_impl_cmp_l(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_l(x, y) mirror_impl_cmp_l(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_l(x, y) mirror_impl_cmp_l(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_l(x, y) mirror_impl_cmp_l(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_l(x, y) mirror_impl_cmp_l(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_l(x, y) mirror_impl_cmp_l(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_l(const char* file, int line, mirror_op_t op,
        long x, const char* sx,
        long y, const char* sy);

/* unsigned long */
#define mirror_eq_ul(x, y) mirror_impl_cmp_ul(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_ul(x, y) mirror_impl_cmp_ul(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_ul(x, y) mirror_impl_cmp_ul(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_ul(x, y) mirror_impl_cmp_ul(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_ul(x, y) mirror_impl_cmp_ul(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_ul(x, y) mirror_impl_cmp_ul(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_ul(const char* file, int line, mirror_op_t op,
        unsigned long x, const char* sx,
        unsigned long y, const char* sy);

/* long long */
#if ghost_has(ghost_llong)
    #define mirror_eq_ll(x, y) mirror_impl_cmp_ll(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
    #define mirror_ne_ll(x, y) mirror_impl_cmp_ll(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
    #define mirror_lt_ll(x, y) mirror_impl_cmp_ll(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
    #define mirror_le_ll(x, y) mirror_impl_cmp_ll(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
    #define mirror_gt_ll(x, y) mirror_impl_cmp_ll(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
    #define mirror_ge_ll(x, y) mirror_impl_cmp_ll(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
    void mirror_impl_cmp_ll(const char* file, int line, mirror_op_t op,
            ghost_llong x, const char* sx,
            ghost_llong y, const char* sy);
#endif

/* unsigned long long */
#if ghost_has(ghost_ullong)
    #define mirror_eq_ull(x, y) mirror_impl_cmp_ull(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
    #define mirror_ne_ull(x, y) mirror_impl_cmp_ull(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
    #define mirror_lt_ull(x, y) mirror_impl_cmp_ull(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
    #define mirror_le_ull(x, y) mirror_impl_cmp_ull(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
    #define mirror_gt_ull(x, y) mirror_impl_cmp_ull(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
    #define mirror_ge_ull(x, y) mirror_impl_cmp_ull(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
    void mirror_impl_cmp_ull(const char* file, int line, mirror_op_t op,
            ghost_ullong x, const char* sx,
            ghost_ullong y, const char* sy);
#endif



/* fixed-width types */

/* int8_t */
#define mirror_eq_i8(x, y) mirror_impl_cmp_i8(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_i8(x, y) mirror_impl_cmp_i8(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_i8(x, y) mirror_impl_cmp_i8(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_i8(x, y) mirror_impl_cmp_i8(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_i8(x, y) mirror_impl_cmp_i8(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_i8(x, y) mirror_impl_cmp_i8(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_i8(const char* file, int line, mirror_op_t op,
        ghost_int8_t x, const char* sx,
        ghost_int8_t y, const char* sy);

/* ghost_uint8_t */
#define mirror_eq_u8(x, y) mirror_impl_cmp_u8(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_u8(x, y) mirror_impl_cmp_u8(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_u8(x, y) mirror_impl_cmp_u8(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_u8(x, y) mirror_impl_cmp_u8(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_u8(x, y) mirror_impl_cmp_u8(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_u8(x, y) mirror_impl_cmp_u8(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_u8(const char* file, int line, mirror_op_t op,
        ghost_uint8_t x, const char* sx,
        ghost_uint8_t y, const char* sy);

/* ghost_int16_t */
#define mirror_eq_i16(x, y) mirror_impl_cmp_i16(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_i16(x, y) mirror_impl_cmp_i16(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_i16(x, y) mirror_impl_cmp_i16(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_i16(x, y) mirror_impl_cmp_i16(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_i16(x, y) mirror_impl_cmp_i16(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_i16(x, y) mirror_impl_cmp_i16(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_i16(const char* file, int line, mirror_op_t op,
        ghost_int16_t x, const char* sx,
        ghost_int16_t y, const char* sy);

/* ghost_uint16_t */
#define mirror_eq_u16(x, y) mirror_impl_cmp_u16(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_u16(x, y) mirror_impl_cmp_u16(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_u16(x, y) mirror_impl_cmp_u16(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_u16(x, y) mirror_impl_cmp_u16(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_u16(x, y) mirror_impl_cmp_u16(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_u16(x, y) mirror_impl_cmp_u16(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_u16(const char* file, int line, mirror_op_t op,
        ghost_uint16_t x, const char* sx,
        ghost_uint16_t y, const char* sy);

/* ghost_int32_t */
#define mirror_eq_i32(x, y) mirror_impl_cmp_i32(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_i32(x, y) mirror_impl_cmp_i32(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_i32(x, y) mirror_impl_cmp_i32(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_i32(x, y) mirror_impl_cmp_i32(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_i32(x, y) mirror_impl_cmp_i32(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_i32(x, y) mirror_impl_cmp_i32(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_i32(const char* file, int line, mirror_op_t op,
        ghost_int32_t x, const char* sx,
        ghost_int32_t y, const char* sy);

/* ghost_uint32_t */
#define mirror_eq_u32(x, y) mirror_impl_cmp_u32(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_u32(x, y) mirror_impl_cmp_u32(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_u32(x, y) mirror_impl_cmp_u32(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_u32(x, y) mirror_impl_cmp_u32(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_u32(x, y) mirror_impl_cmp_u32(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_u32(x, y) mirror_impl_cmp_u32(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_u32(const char* file, int line, mirror_op_t op,
        ghost_uint32_t x, const char* sx,
        ghost_uint32_t y, const char* sy);

/* ghost_int64_t */
#define mirror_eq_i64(x, y) mirror_impl_cmp_i64(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_i64(x, y) mirror_impl_cmp_i64(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_i64(x, y) mirror_impl_cmp_i64(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_i64(x, y) mirror_impl_cmp_i64(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_i64(x, y) mirror_impl_cmp_i64(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_i64(x, y) mirror_impl_cmp_i64(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_i64(const char* file, int line, mirror_op_t op,
        ghost_int64_t x, const char* sx,
        ghost_int64_t y, const char* sy);

/* ghost_uint64_t */
#define mirror_eq_u64(x, y) mirror_impl_cmp_u64(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_u64(x, y) mirror_impl_cmp_u64(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_u64(x, y) mirror_impl_cmp_u64(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_u64(x, y) mirror_impl_cmp_u64(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_u64(x, y) mirror_impl_cmp_u64(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_u64(x, y) mirror_impl_cmp_u64(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_u64(const char* file, int line, mirror_op_t op,
        ghost_uint64_t x, const char* sx,
        ghost_uint64_t y, const char* sy);



/* pointer or size-sized types */

/* size_t */
#define mirror_eq_z(x, y) mirror_impl_cmp_z(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_z(x, y) mirror_impl_cmp_z(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_z(x, y) mirror_impl_cmp_z(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_z(x, y) mirror_impl_cmp_z(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_z(x, y) mirror_impl_cmp_z(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_z(x, y) mirror_impl_cmp_z(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_z(const char* file, int line, mirror_op_t op,
        ghost_size_t x, const char* sx,
        ghost_size_t y, const char* sy);

/* ssize_t */
#define mirror_eq_sz(x, y) mirror_impl_cmp_sz(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_sz(x, y) mirror_impl_cmp_sz(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_sz(x, y) mirror_impl_cmp_sz(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_sz(x, y) mirror_impl_cmp_sz(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_sz(x, y) mirror_impl_cmp_sz(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_sz(x, y) mirror_impl_cmp_sz(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_sz(const char* file, int line, mirror_op_t op,
        ghost_ssize_t x, const char* sx,
        ghost_ssize_t y, const char* sy);

/* intptr_t */
#define mirror_eq_ip(x, y) mirror_impl_cmp_ip(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_ip(x, y) mirror_impl_cmp_ip(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_ip(x, y) mirror_impl_cmp_ip(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_ip(x, y) mirror_impl_cmp_ip(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_ip(x, y) mirror_impl_cmp_ip(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_ip(x, y) mirror_impl_cmp_ip(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_ip(const char* file, int line, mirror_op_t op,
        ghost_intptr_t x, const char* sx,
        ghost_intptr_t y, const char* sy);

/* uintptr_t */
#define mirror_eq_up(x, y) mirror_impl_cmp_up(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_up(x, y) mirror_impl_cmp_up(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_up(x, y) mirror_impl_cmp_up(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_up(x, y) mirror_impl_cmp_up(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_up(x, y) mirror_impl_cmp_up(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_up(x, y) mirror_impl_cmp_up(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_up(const char* file, int line, mirror_op_t op,
        ghost_uintptr_t x, const char* sx,
        ghost_uintptr_t y, const char* sy);

/* ptrdiff_t */
#define mirror_eq_pd(x, y) mirror_impl_cmp_pd(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_pd(x, y) mirror_impl_cmp_pd(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_pd(x, y) mirror_impl_cmp_pd(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_pd(x, y) mirror_impl_cmp_pd(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_pd(x, y) mirror_impl_cmp_pd(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_pd(x, y) mirror_impl_cmp_pd(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_pd(const char* file, int line, mirror_op_t op,
        ghost_ptrdiff_t x, const char* sx,
        ghost_ptrdiff_t y, const char* sy);



/* higher-level types */

/* const char* (null-terminated string) */
#define mirror_eq_s(x, y) mirror_impl_cmp_s(__FILE__, __LINE__, mirror_op_eq, x, #x, y, #y)
#define mirror_ne_s(x, y) mirror_impl_cmp_s(__FILE__, __LINE__, mirror_op_ne, x, #x, y, #y)
#define mirror_lt_s(x, y) mirror_impl_cmp_s(__FILE__, __LINE__, mirror_op_lt, x, #x, y, #y)
#define mirror_le_s(x, y) mirror_impl_cmp_s(__FILE__, __LINE__, mirror_op_le, x, #x, y, #y)
#define mirror_gt_s(x, y) mirror_impl_cmp_s(__FILE__, __LINE__, mirror_op_gt, x, #x, y, #y)
#define mirror_ge_s(x, y) mirror_impl_cmp_s(__FILE__, __LINE__, mirror_op_ge, x, #x, y, #y)
void mirror_impl_cmp_s(const char* file, int line, mirror_op_t op,
        const char* x, const char* sx,
        const char* y, const char* sy);

/* pointer comparison */
/* Any pointer type is forwarded directly to uintptr_t.
 * We need an intermediate cast to void* because MSVC 2013 is C++98 and doesn't
 * allow a reinterpret cast of NULL directly to uintptr_t. */
#define mirror_eq_p(x, y) mirror_impl_cmp_up(__FILE__, __LINE__, mirror_op_eq, \
        ghost_reinterpret_cast(uintptr_t, ghost_static_cast(void*, x)), #x, \
        ghost_reinterpret_cast(uintptr_t, ghost_static_cast(void*, y)), #y)
#define mirror_ne_p(x, y) mirror_impl_cmp_up(__FILE__, __LINE__, mirror_op_ne, \
        ghost_reinterpret_cast(uintptr_t, ghost_static_cast(void*, x)), #x, \
        ghost_reinterpret_cast(uintptr_t, ghost_static_cast(void*, y)), #y)
#define mirror_lt_p(x, y) mirror_impl_cmp_up(__FILE__, __LINE__, mirror_op_lt, \
        ghost_reinterpret_cast(uintptr_t, ghost_static_cast(void*, x)), #x, \
        ghost_reinterpret_cast(uintptr_t, ghost_static_cast(void*, y)), #y)
#define mirror_le_p(x, y) mirror_impl_cmp_up(__FILE__, __LINE__, mirror_op_le, \
        ghost_reinterpret_cast(uintptr_t, ghost_static_cast(void*, x)), #x, \
        ghost_reinterpret_cast(uintptr_t, ghost_static_cast(void*, y)), #y)
#define mirror_gt_p(x, y) mirror_impl_cmp_up(__FILE__, __LINE__, mirror_op_gt, \
        ghost_reinterpret_cast(uintptr_t, ghost_static_cast(void*, x)), #x, \
        ghost_reinterpret_cast(uintptr_t, ghost_static_cast(void*, y)), #y)
#define mirror_ge_p(x, y) mirror_impl_cmp_up(__FILE__, __LINE__, mirror_op_ge, \
        ghost_reinterpret_cast(uintptr_t, ghost_static_cast(void*, x)), #x, \
        ghost_reinterpret_cast(uintptr_t, ghost_static_cast(void*, y)), #y)



#ifdef __cplusplus
}
#endif

#endif
