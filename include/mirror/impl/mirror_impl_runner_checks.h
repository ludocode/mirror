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

#ifndef MIRROR_IMPL_RUNNER_CHECKS_H
#define MIRROR_IMPL_RUNNER_CHECKS_H

#include "mirror/impl/mirror_impl_checks.h"

#ifdef __cplusplus
extern "C" {
#endif

static ghost_bool mirror_op_test(mirror_op_t op, int cmp) {
    switch (op) {
        case mirror_op_eq: return cmp == 0;
        case mirror_op_ne: return cmp != 0;
        case mirror_op_lt: return cmp <  0;
        case mirror_op_le: return cmp <= 0;
        case mirror_op_gt: return cmp >  0;
        case mirror_op_ge: return cmp >= 0;
        default: ghost_fatal("invalid op for cmp test");
    }
    ghost_unreachable(ghost_false);
}

static const char* mirror_op_to_string(mirror_op_t op) {
    switch (op) {
        /*
        case mirror_op_eq: return "equal";
        case mirror_op_ne: return "not equal";
        case mirror_op_lt: return "less than";
        case mirror_op_le: return "less than or equal";
        case mirror_op_gt: return "greater than";
        case mirror_op_ge: return "greater than or equal";
        */
        case mirror_op_eq: return "==";
        case mirror_op_ne: return "!=";
        case mirror_op_lt: return "<";
        case mirror_op_le: return "<=";
        case mirror_op_gt: return ">";
        case mirror_op_ge: return ">=";
        case mirror_op_eqb: return "equal bitwise";
        case mirror_op_eqe: return "equal (within acceptable error)";
    }
    ghost_unreachable("");
}

void mirror_fail_cmp(const char* file, int line, mirror_op_t op,
        const char* fx, const char* sx, const char* fy, const char* sy)
{
    char message[1024];
    ghost_bool x_match = strcmp(fx, sx) == 0;
    ghost_bool y_match = strcmp(fy, sy) == 0;
    if (x_match && y_match) {
        ghost_snprintf(message, sizeof(message), "Assertion failed!\n"
                "Expected %s:\n"
                "    %s\n"
                "    %s\n",
                mirror_op_to_string(op), fx, fy);
    } else if (x_match) {
        ghost_snprintf(message, sizeof(message), "Assertion failed!\n"
                "Expected %s:\n"
                "    %s\n"
                "    %s\n"
                "        %s\n",
                mirror_op_to_string(op), fx, fy, sy);
    } else if (y_match) {
        ghost_snprintf(message, sizeof(message), "Assertion failed!\n"
                "Expected %s:\n"
                "    %s\n"
                "        %s\n"
                "    %s\n",
                mirror_op_to_string(op), fx, sx, fy);
    } else {
        ghost_snprintf(message, sizeof(message), "Assertion failed!\n"
                "Expected %s:\n"
                "    %s\n"
                "        %s\n"
                "    %s\n"
                "        %s\n",
                mirror_op_to_string(op), fx, sx, fy, sy);
    }
    mirror_handle_failure(file, line, message);
}



/* chars */

/* char, may be signed or unsigned */
void mirror_impl_cmp_c(const char* file, int line, mirror_op_t op,
        char x, const char* sx,
        char y, const char* sy)
{
    #if ghost_has(GHOST_CHAR_IS_SIGNED)
        #if GHOST_CHAR_IS_SIGNED
            /* no casts; we want warnings if signedness is wrong. */
            mirror_impl_cmp_sc(file, line, op, x, sx, y, sy);
        #else
            /* no casts; we want warnings if signedness is wrong. */
            mirror_impl_cmp_uc(file, line, op, x, sx, y, sy);
        #endif
    #else
        /* determine signedness at runtime */
        if (ghost_static_cast(char, -1) < 0) {
            mirror_impl_cmp_sc(file, line, op,
                    ghost_static_cast(signed char, x), sx,
                    ghost_static_cast(signed char, y), sy);
        } else {
            mirror_impl_cmp_uc(file, line, op,
                    ghost_static_cast(unsigned char, x), sx,
                    ghost_static_cast(unsigned char, y), sy);
        }
    #endif
}

/* signed char */
void mirror_impl_cmp_sc(const char* file, int line, mirror_op_t op,
        signed char x, const char* sx,
        signed char y, const char* sy)
{
    ghost_bool x_ascii;
    ghost_bool y_ascii;
    char fx[64];
    char fy[64];

    if (ghost_expect_true(mirror_op_test(op, ghost_compare_sc(x, y))))
        return;

    x_ascii = x >= 0x21 && x <= 0x7e;
    y_ascii = y >= 0x21 && y <= 0x7e;

    if (x_ascii)
        ghost_snprintf(fx, sizeof(fx), "%i '%c' (0x%02x)",
                ghost_static_cast(int, x),
                ghost_static_cast(int, x),
                ghost_static_cast(unsigned int, x));
    else
        ghost_snprintf(fx, sizeof(fx), "%i (0x%02x)",
                ghost_static_cast(int, x),
                ghost_static_cast(unsigned int, x));

    if (y_ascii)
        ghost_snprintf(fy, sizeof(fy), "%i '%c' (0x%02x)",
                ghost_static_cast(int, y),
                ghost_static_cast(int, y),
                ghost_static_cast(unsigned int, y));
    else
        ghost_snprintf(fy, sizeof(fy), "%i (0x%02x)",
                ghost_static_cast(int, y),
                ghost_static_cast(unsigned int, y));

    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* unsigned char */
void mirror_impl_cmp_uc(const char* file, int line, mirror_op_t op,
        unsigned char x, const char* sx,
        unsigned char y, const char* sy)
{
    ghost_bool x_ascii;
    ghost_bool y_ascii;
    char fx[64];
    char fy[64];

    if (ghost_expect_true(mirror_op_test(op, ghost_compare_uc(x, y))))
        return;

    x_ascii = x >= 0x21 && x <= 0x7e;
    y_ascii = y >= 0x21 && y <= 0x7e;

    if (x_ascii)
        ghost_snprintf(fx, sizeof(fx), "%u '%c' (0x%02x)",
                ghost_static_cast(unsigned int, x),
                ghost_static_cast(unsigned int, x),
                ghost_static_cast(unsigned int, x));
    else
        ghost_snprintf(fx, sizeof(fx), "%u (0x%02x)",
                ghost_static_cast(unsigned int, x),
                ghost_static_cast(unsigned int, x));

    if (y_ascii)
        ghost_snprintf(fy, sizeof(fy), "%u '%c' (0x%02x)",
                ghost_static_cast(unsigned int, y),
                ghost_static_cast(unsigned int, y),
                ghost_static_cast(unsigned int, y));
    else
        ghost_snprintf(fy, sizeof(fy), "%u (0x%02x)",
                ghost_static_cast(unsigned int, y),
                ghost_static_cast(unsigned int, y));

    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* char16_t */
void mirror_impl_cmp_c16(const char* file, int line, mirror_op_t op,
        ghost_char16_t x, const char* sx,
        ghost_char16_t y, const char* sy)
{
    ghost_bool x_ascii;
    ghost_bool y_ascii;
    char fx[64];
    char fy[64];

    if (ghost_expect_true(mirror_op_test(op, ghost_compare_f(x, y))))
        return;

    x_ascii = x >= 0x21 && x <= 0x7e;
    y_ascii = y >= 0x21 && y <= 0x7e;

    if (x_ascii)
        ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIu16 " '%c' (0x%02" GHOST_PRIx16 ")",
                ghost_static_cast(ghost_uint16_t, x),
                ghost_static_cast(ghost_uint16_t, x),
                ghost_static_cast(char, x));
    else
        ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIu16 " (0x%02" GHOST_PRIx16 ")",
                ghost_static_cast(ghost_uint16_t, x),
                ghost_static_cast(ghost_uint16_t, x));

    if (y_ascii)
        ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIu16 " '%c' (0x%02" GHOST_PRIx16 ")",
                ghost_static_cast(ghost_uint16_t, y),
                ghost_static_cast(ghost_uint16_t, y),
                ghost_static_cast(char, y));
    else
        ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIu16 " (0x%02" GHOST_PRIx16 ")",
                ghost_static_cast(ghost_uint16_t, y),
                ghost_static_cast(ghost_uint16_t, y));

    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* char32_t */
void mirror_impl_cmp_c32(const char* file, int line, mirror_op_t op,
        ghost_char32_t x, const char* sx,
        ghost_char32_t y, const char* sy)
{
    ghost_bool x_ascii;
    ghost_bool y_ascii;
    char fx[64];
    char fy[64];

    if (ghost_expect_true(mirror_op_test(op, ghost_compare_c32(x, y))))
        return;

    x_ascii = x >= 0x21 && x <= 0x7e;
    y_ascii = y >= 0x21 && y <= 0x7e;

    if (x_ascii)
        ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIu32 " '%c' (0x%02" GHOST_PRIx32 ")",
                ghost_static_cast(ghost_uint32_t, x),
                ghost_static_cast(ghost_uint32_t, x),
                ghost_static_cast(char, x));
    else
        ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIu32 " (0x%02" GHOST_PRIx32 ")",
                ghost_static_cast(ghost_uint32_t, x),
                ghost_static_cast(ghost_uint32_t, x));

    if (y_ascii)
        ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIu32 " '%c' (0x%02" GHOST_PRIx32 ")",
                ghost_static_cast(ghost_uint32_t, y),
                ghost_static_cast(ghost_uint32_t, y),
                ghost_static_cast(char, y));
    else
        ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIu32 " (0x%02" GHOST_PRIx32 ")",
                ghost_static_cast(ghost_uint32_t, y),
                ghost_static_cast(ghost_uint32_t, y));

    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}



/* floats */

/* float */
#if ghost_has(ghost_float)
    void mirror_impl_cmp_f(const char* file, int line, mirror_op_t op,
            float x, const char* sx,
            float y, const char* sy)
    {
        char fx[32];
        char fy[32];
        /* TODO we should either compare equality for floats bitwise or have an
         * alternate name for comparing them that way (eqb?) */
        if (ghost_expect_true(mirror_op_test(op, ghost_compare_f(x, y))))
            return;
        ghost_snprintf(fx, sizeof(fx), "%g", x);
        ghost_snprintf(fy, sizeof(fy), "%g", y);
        mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
    }

    void mirror_impl_eqe_f(const char* file, int line,
            float x, const char* sx,
            float y, const char* sy,
            float error, const char* serror)
    {
        char fx[32];
        char fy[32];
        float epsilon = 0.0001F;
        float denom = ghost_max_f(ghost_max_f(x, y), epsilon); /* prevent divide by zero */
        error = ghost_max_f(error, epsilon); /* minimum error */
        if (ghost_expect_true(ghost_abs_f((x - y) / denom) < error))
            return;
        ghost_snprintf(fx, sizeof(fx), "%g", x);
        ghost_snprintf(fy, sizeof(fy), "%g", y);
        /* TODO pass error along */
        ghost_discard(serror);
        mirror_fail_cmp(file, line, mirror_op_eqe, fx, sx, fy, sy);
    }
#endif

/* double */
#if ghost_has(ghost_double)
    void mirror_impl_cmp_d(const char* file, int line, mirror_op_t op,
            double x, const char* sx,
            double y, const char* sy)
    {
        char fx[32];
        char fy[32];
        if (ghost_expect_true(mirror_op_test(op, ghost_compare_d(x, y))))
            return;
        ghost_snprintf(fx, sizeof(fx), "%g", x);
        ghost_snprintf(fy, sizeof(fy), "%g", y);
        mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
    }

    void mirror_impl_eqe_d(const char* file, int line,
            double x, const char* sx,
            double y, const char* sy,
            double error, const char* serror)
    {
        char fx[32];
        char fy[32];
        double epsilon = 0.0001f;
        double denom = ghost_max_d(ghost_max_d(x, y), epsilon); /* prevent divide by zero */
        error = ghost_max_d(error, epsilon); /* minimum error */
        if (ghost_expect_true(ghost_abs_d((x - y) / denom) < error))
            return;
        ghost_snprintf(fx, sizeof(fx), "%g", x);
        ghost_snprintf(fy, sizeof(fy), "%g", y);
        /* TODO pass error along */
        ghost_discard(serror);
        mirror_fail_cmp(file, line, mirror_op_eqe, fx, sx, fy, sy);
    }
#endif

/* long double */
#if ghost_has(ghost_ldouble)
    void mirror_impl_cmp_ld(const char* file, int line, mirror_op_t op,
            long double x, const char* sx,
            long double y, const char* sy)
    {
        char fx[32];
        char fy[32];
        if (ghost_expect_true(mirror_op_test(op, ghost_compare_ld(x, y))))
            return;
        ghost_snprintf(fx, sizeof(fx), "%Lg", x);
        ghost_snprintf(fy, sizeof(fy), "%Lg", y);
        mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
    }
#endif

/* float32_t */
#if ghost_has(ghost_float32_t)
    void mirror_impl_cmp_f32(const char* file, int line, mirror_op_t op,
            ghost_float32_t x, const char* sx,
            ghost_float32_t y, const char* sy)
    {
        char fx[32];
        char fy[32];
        if (ghost_expect_true(mirror_op_test(op, ghost_compare_f32(x, y))))
            return;
        /* TODO still need to implement e.g. ghost_format_f32.h */
        ghost_snprintf(fx, sizeof(fx), "%g"/*TODO proper specifier*/, x);
        ghost_snprintf(fy, sizeof(fy), "%g"/*TODO proper specifier*/, y);
        mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
    }
#endif

/* float64_t */
#if ghost_has(ghost_float64_t)
    void mirror_impl_cmp_f64(const char* file, int line, mirror_op_t op,
            ghost_float64_t x, const char* sx,
            ghost_float64_t y, const char* sy)
    {
        char fx[32];
        char fy[32];
        if (ghost_expect_true(mirror_op_test(op, ghost_compare_f64(x, y))))
            return;
        /* TODO still need to implement e.g. ghost_format_f64.h */
        ghost_snprintf(fx, sizeof(fx), "%g"/*TODO proper specifier*/, x);
        ghost_snprintf(fy, sizeof(fy), "%g"/*TODO proper specifier*/, y);
        mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
    }
#endif



/* other basic types: short, int, long, long long */

/* short */
void mirror_impl_cmp_h(const char* file, int line, mirror_op_t op,
        short x, const char* sx,
        short y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_h(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%hi", x);
    ghost_snprintf(fy, sizeof(fy), "%hi", y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* unsigned short */
void mirror_impl_cmp_uh(const char* file, int line, mirror_op_t op,
        unsigned short x, const char* sx,
        unsigned short y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_uh(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%hu", x);
    ghost_snprintf(fy, sizeof(fy), "%hu", y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* int */
void mirror_impl_cmp_i(const char* file, int line, mirror_op_t op,
        int x, const char* sx,
        int y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_i(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%i", x);
    ghost_snprintf(fy, sizeof(fy), "%i", y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* unsigned (int) */
void mirror_impl_cmp_u(const char* file, int line, mirror_op_t op,
        unsigned x, const char* sx,
        unsigned y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_u(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%u", x);
    ghost_snprintf(fy, sizeof(fy), "%u", y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* long */
void mirror_impl_cmp_l(const char* file, int line, mirror_op_t op,
        long x, const char* sx,
        long y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_l(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%li", x);
    ghost_snprintf(fy, sizeof(fy), "%li", y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* unsigned long */
void mirror_impl_cmp_ul(const char* file, int line, mirror_op_t op,
        unsigned long x, const char* sx,
        unsigned long y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_ul(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%lu", x);
    ghost_snprintf(fy, sizeof(fy), "%lu", y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* long long */
#if ghost_has(ghost_llong)
void mirror_impl_cmp_ll(const char* file, int line, mirror_op_t op,
        ghost_llong x, const char* sx,
        ghost_llong y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_ll(x, y))))
        return;
    #if GHOST_IMPL_FORMAT_C99_SPECIFIERS
        ghost_snprintf(fx, sizeof(fx), "%lli", x);
        ghost_snprintf(fy, sizeof(fy), "%lli", y);
    #else
        /* TODO assuming it fits */
        ghost_snprintf(fx, sizeof(fx), "%li", ghost_static_cast(long, x));
        ghost_snprintf(fy, sizeof(fy), "%li", ghost_static_cast(long, y));
    #endif
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}
#endif

/* unsigned long long */
#if ghost_has(ghost_ullong)
void mirror_impl_cmp_ull(const char* file, int line, mirror_op_t op,
        ghost_ullong x, const char* sx,
        ghost_ullong y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_ull(x, y))))
        return;
    #if GHOST_IMPL_FORMAT_C99_SPECIFIERS
        ghost_snprintf(fx, sizeof(fx), "%llu", x);
        ghost_snprintf(fy, sizeof(fy), "%llu", y);
    #else
        /* TODO assuming it fits */
        ghost_snprintf(fx, sizeof(fx), "%lu", ghost_static_cast(unsigned long, x));
        ghost_snprintf(fy, sizeof(fy), "%lu", ghost_static_cast(unsigned long, y));
    #endif
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}
#endif



/* fixed-width types */

/* int8_t */
void mirror_impl_cmp_i8(const char* file, int line, mirror_op_t op,
        ghost_int8_t x, const char* sx,
        ghost_int8_t y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_i8(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%i", ghost_static_cast(int, x));
    ghost_snprintf(fy, sizeof(fy), "%i", ghost_static_cast(int, y));
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* ghost_uint8_t */
void mirror_impl_cmp_u8(const char* file, int line, mirror_op_t op,
        ghost_uint8_t x, const char* sx,
        ghost_uint8_t y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_u8(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%u", ghost_static_cast(unsigned int, x));
    ghost_snprintf(fy, sizeof(fy), "%u", ghost_static_cast(unsigned int, y));
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* ghost_int16_t */
void mirror_impl_cmp_i16(const char* file, int line, mirror_op_t op,
        ghost_int16_t x, const char* sx,
        ghost_int16_t y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_i16(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIi16, x);
    ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIi16, y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* ghost_uint16_t */
void mirror_impl_cmp_u16(const char* file, int line, mirror_op_t op,
        ghost_uint16_t x, const char* sx,
        ghost_uint16_t y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_u16(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIu16, x);
    ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIu16, y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* ghost_int32_t */
void mirror_impl_cmp_i32(const char* file, int line, mirror_op_t op,
        ghost_int32_t x, const char* sx,
        ghost_int32_t y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_i32(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIi32, x);
    ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIi32, y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* ghost_uint32_t */
void mirror_impl_cmp_u32(const char* file, int line, mirror_op_t op,
        ghost_uint32_t x, const char* sx,
        ghost_uint32_t y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_u32(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIu32, x);
    ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIu32, y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* ghost_int64_t */
void mirror_impl_cmp_i64(const char* file, int line, mirror_op_t op,
        ghost_int64_t x, const char* sx,
        ghost_int64_t y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_i64(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIi64, x);
    ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIi64, y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* ghost_uint64_t */
void mirror_impl_cmp_u64(const char* file, int line, mirror_op_t op,
        ghost_uint64_t x, const char* sx,
        ghost_uint64_t y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_u64(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIu64, x);
    ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIu64, y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}



/* pointer or size-sized types */

/* size_t */
void mirror_impl_cmp_z(const char* file, int line, mirror_op_t op,
        ghost_size_t x, const char* sx,
        ghost_size_t y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_z(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIuZ, x);
    ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIuZ, y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* ssize_t */
void mirror_impl_cmp_sz(const char* file, int line, mirror_op_t op,
        ghost_ssize_t x, const char* sx,
        ghost_ssize_t y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_sz(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIiZ, x);
    ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIiZ, y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* intptr_t */
void mirror_impl_cmp_ip(const char* file, int line, mirror_op_t op,
        ghost_intptr_t x, const char* sx,
        ghost_intptr_t y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_ip(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIiP, x);
    ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIiP, y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* uintptr_t
 * This is printed as hexadecimal since it's a pointer-sized int, probably
 * storing a pointer. This is also used for arbitrary pointers. */
void mirror_impl_cmp_up(const char* file, int line, mirror_op_t op,
        ghost_uintptr_t x, const char* sx,
        ghost_uintptr_t y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_up(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "0x%" GHOST_PRIxP, x);
    ghost_snprintf(fy, sizeof(fy), "0x%" GHOST_PRIxP, y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}

/* ptrdiff_t */
void mirror_impl_cmp_pd(const char* file, int line, mirror_op_t op,
        ghost_ptrdiff_t x, const char* sx,
        ghost_ptrdiff_t y, const char* sy)
{
    char fx[32];
    char fy[32];
    if (ghost_expect_true(mirror_op_test(op, ghost_compare_pd(x, y))))
        return;
    ghost_snprintf(fx, sizeof(fx), "%" GHOST_PRIiPD, x);
    ghost_snprintf(fy, sizeof(fy), "%" GHOST_PRIiPD, y);
    mirror_fail_cmp(file, line, op, fx, sx, fy, sy);
}



/* higher-level types */

/* const char* (null-terminated string) */
void mirror_impl_cmp_s(const char* file, int line, mirror_op_t op,
        const char* x, const char* sx,
        const char* y, const char* sy)
{
    if (ghost_expect_true(mirror_op_test(op, ghost_strcmp(x, y))))
        return;
    mirror_fail_cmp(file, line, op, x, sx, y, sy);
}

#ifdef __cplusplus
}
#endif

#endif
