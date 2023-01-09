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

#ifndef MIRROR_IMPL_DECLARE_H
#define MIRROR_IMPL_DECLARE_H

#include "mirror/impl/mirror_impl_ghost.h"

#ifdef __cplusplus
extern "C" {
#endif



/*
 * Test defaults
 */

#ifndef MIRROR_KEY
    #if GHOST_GCC || defined(__clang__)
        /* We don't need unique ids under GCC/Clang because
         * __attribute__((constructor)) works on static functions. */
        #define MIRROR_KEY /*nothing*/
    #else
        /* TODO required if no static constructor
         * TODO actually shouldn't make it required? should be able to provide id() param for test id */
        #define MIRROR_KEY /*()ERROR_MIRROR_ID_NOT_DEFINED_AND_NO_ID_OPTION_PROVIDED()*/
        /*#error */
    #endif
#endif

/* TODO the idea here is that you can define a name for all tests, then add to
 * the name using suffix() for variant tests. suffix() isn't implemented yet. */
#ifndef MIRROR_NAME
    #define MIRROR_NAME "<anonymous>"
#endif



/**
 * A test suite.
 */
typedef struct mirror_suite_t mirror_suite_t;

/**
 * A test case.
 */
typedef struct mirror_test_t mirror_test_t;

/*
 * A thunk for a test case function or fixture setup/teardown function.
 *
 * The thunk casts the fixture and param arguments to their correct types.
 */
typedef void (*mirror_thunk_t)(void* fixture, void* param);

/*
 * A test suite setup/teardown function.
 *
 * TODO not yet.
 */
/*typedef void (*mirror_suite_thunk_t)(mirror_suite_t* test_suite); */

struct mirror_suite_t {

    /* options */
    const char* name;
    ghost_bool smoke; /* TODO replace with priority? */
    ghost_bool skip;
    ghost_bool death;
    const char* description;
    const char** deps;
    size_t deps_count;
    /*TODO case_setup;
     *TODO case_teardown; */

    /*mirror_suite_thunk_t suite_setup; // TODO not yet.
     *mirror_suite_thunk_t suite_teardown; // TODO not yet. */

    ghost_size_t fixture_size;
    void (*fixture_setup)(void*);
    void (*fixture_teardown)(void*);

    /* links */
    ghost_iwbt_node_t all_suites;
    ghost_iwbt_node_t suite_suites;
    /*
    mirror_suite_t* parent;
    mirror_test_t* first_test;
    mirror_suite_t* first_suite;
    mirror_suite_t* next_globally;
    mirror_suite_t* next_in_parent;
    */

    /* misc */
    ghost_bool allocated;
};

struct mirror_test_t {

    /* options */
    const char* id;
    const char* name;
    mirror_thunk_t fn;
    ghost_bool death;
    ghost_bool smoke;
    ghost_bool skip;
    const char* file;
    int line;
    const char* description;
    const char** deps;
    size_t deps_count;

    ghost_size_t fixture_size;
    void (*fixture_setup)(void*);
    void (*fixture_teardown)(void*);

    /* links */
    mirror_suite_t* suite;
    ghost_iwbt_node_t all_tests;
    ghost_iwbt_node_t suite_tests;
};

void mirror_register_test(mirror_test_t* test);



/*
 * MIRROR_REGISTRATION_BLOCK() is a static construction block which is emulated
 * on compilers that don't support it.
 *
 * We use ghost_static_init() which is just __attribute__((constructor)) or an
 * emulation of it. This makes the blocks run automatically.
 *
 * Some compilers like TinyCC don't support this so we use a workaround that
 * requires buildsystem support. The registration blocks in this case are
 * simply extern functions starting with mirror_REGISTER_. After compiling all
 * source files, a script runs through all object files looking for functions
 * that start with mirror_REGISTER_. It then generates a source file with a
 * function called mirror_register_all() which just calls all functions
 * starting with mirror_REGISTER_. Mirror then calls mirror_register_all() on
 * startup.
 */
#if ghost_has(ghost_static_init)
    #define MIRROR_REGISTRATION_BLOCK(id) \
            ghost_static_init(GHOST_CONCAT(mirror_REGISTER_, id))
#else
    #define MIRROR_REGISTRATION_BLOCK(id) \
            void GHOST_CONCAT(mirror_REGISTER_, id)(void); \
            void GHOST_CONCAT(mirror_REGISTER_, id)(void)
void mirror_register_all(void);
#endif



/* TODO */

#define MIRROR_EXPAND(x) x



/**
 * @def mirror(...)
 *
 * This requires that your preprocessor support variadic macros. If not, you
 * need to use mirror_N() where N is the number of arguments.
 */

#if GHOST_PP_VA_ARGS
    #if GHOST_CPARSER_PP
        /* I'm not even going to try to implement mirror under cparser's busted
         * preprocessor. Instead we just ignore all arguments and forward
         * straight to the nofixture noparam implementation. You'll have to
         * manually wrap and disable any tests that need custom options like
         * fixtures and params under cparser. */
        #define mirror(...) MIRROR_IMPL_mirror_fixtureNONE_mirror_paramNONE(GHOST_CONCAT(MIRROR_KEY, GHOST_COUNTER),,,,,,,,,,,,)

    #elif GHOST_MSVC_TRADITIONAL
        #define mirror(...) GHOST_EXPAND(GHOST_CONCAT(mirror_, GHOST_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__))

    #else
        #define mirror(...) GHOST_CONCAT(mirror_, GHOST_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)
    #endif
#else
    /* Most tests (at least in Ghost) have no arguments so we allow mirror() to
     * be used for that. */
    #define mirror mirror_0
#endif

/*TODO use GHOST_CONCAT as above
     *#define mirror(...) mirror_impl(GHOST_COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)
     *#define mirror_impl(N, ...) mirror_impl2(N, __VA_ARGS__)
     *#define mirror_impl2(N, ...) mirror_##N(__VA_ARGS__)
 *#endif */


/* TODO this shouldn't be necessary, under msvc traditional make mirror() forward to mirror_0() */
#ifdef _MSC_VER
__pragma(warning(disable:4003))
#endif



/*
 * The entire rest of this file is written for maximum preprocessor
 * compatibility:
 *
 * - Variadic macros are not used (strict ANSI C is supported);
 * - Empty macro arguments are not used to avoid MSVC warnings (mirror_nothing is used instead);
 * - Other workarounds are added for the MSVC traditional preprocessor (e.g. extra expansions);
 * - Other buggy preprocessors are supported as well (e.g. chibicc, even cparser is partially supported)
 */



/**
 * @def mirror_N()
 */

/* TODO another cparser workaround */
#ifdef __CPARSER__
#define mirror_0() MIRROR_IMPL_mirror_fixtureNONE_mirror_paramNONE(GHOST_CONCAT(MIRROR_KEY, GHOST_COUNTER),,,,,,,,,,,,)
#define mirror_1(a) MIRROR_IMPL_mirror_fixtureNONE_mirror_paramNONE(GHOST_CONCAT(MIRROR_KEY, GHOST_COUNTER),,,,,,,,,,,,)
#define mirror_2(a,b) MIRROR_IMPL_mirror_fixtureNONE_mirror_paramNONE(GHOST_CONCAT(MIRROR_KEY, GHOST_COUNTER),,,,,,,,,,,,)
#define mirror_3(a,b,c) MIRROR_IMPL_mirror_fixtureNONE_mirror_paramNONE(GHOST_CONCAT(MIRROR_KEY, GHOST_COUNTER),,,,,,,,,,,,)
#define mirror_4(a,b,c,d) MIRROR_IMPL_mirror_fixtureNONE_mirror_paramNONE(GHOST_CONCAT(MIRROR_KEY, GHOST_COUNTER),,,,,,,,,,,,)
#define mirror_5(a,b,c,d,e) MIRROR_IMPL_mirror_fixtureNONE_mirror_paramNONE(GHOST_CONCAT(MIRROR_KEY, GHOST_COUNTER),,,,,,,,,,,,)
#define mirror_6(a,b,c,d,e,f) MIRROR_IMPL_mirror_fixtureNONE_mirror_paramNONE(GHOST_CONCAT(MIRROR_KEY, GHOST_COUNTER),,,,,,,,,,,,)
#define mirror_7(a,b,c,d,e,f,g) MIRROR_IMPL_mirror_fixtureNONE_mirror_paramNONE(GHOST_CONCAT(MIRROR_KEY, GHOST_COUNTER),,,,,,,,,,,,)
#define mirror_8(a,b,c,d,e,f,g,h) MIRROR_IMPL_mirror_fixtureNONE_mirror_paramNONE(GHOST_CONCAT(MIRROR_KEY, GHOST_COUNTER),,,,,,,,,,,,)
#else

/* This is necessary to prevent warnings from the MSVC traditional
 * preprocessor. You're supposed to be able to pass nothing to a macro that
 * takes one argument but old MSVC warns against it. */
#define mirror_0() \
    MIRROR_IMPL(mirror_nothing, mirror_nothing, mirror_nothing, mirror_nothing, \
             mirror_nothing, mirror_nothing, mirror_nothing, mirror_nothing)

#define mirror_1(a) \
    MIRROR_IMPL(a, mirror_nothing, mirror_nothing, mirror_nothing, \
             mirror_nothing, mirror_nothing, mirror_nothing, mirror_nothing)

#define mirror_2(a, b) \
    MIRROR_IMPL(a, b, mirror_nothing, mirror_nothing, \
             mirror_nothing, mirror_nothing, mirror_nothing, mirror_nothing)

#define mirror_3(a, b, c) \
    MIRROR_IMPL(a, b, c, mirror_nothing, \
             mirror_nothing, mirror_nothing, mirror_nothing, mirror_nothing)

#define mirror_4(a, b, c, d) \
    MIRROR_IMPL(a, b, c, d, \
             mirror_nothing, mirror_nothing, mirror_nothing, mirror_nothing)

#define mirror_5(a, b, c, d, e) \
    MIRROR_IMPL(a, b, c, d, \
             e, mirror_nothing, mirror_nothing, mirror_nothing)

#define mirror_6(a, b, c, d, e, f) \
    MIRROR_IMPL(a, b, c, d, \
             e, f, mirror_nothing, mirror_nothing)

#define mirror_7(a, b, c, d, e, f, g) \
    MIRROR_IMPL(a, b, c, d, \
             e, f, g, mirror_nothing)

#define mirror_8(a, b, c, d, e, f, g, h) \
    MIRROR_IMPL(a, b, c, d, \
             e, f, g, h)

#endif


/**
 * @def MIRROR_EXTRACT(name, fallback, args...)
 *
 * Extracts the contents of an id(), fixture() or param() argument from a
 * parameter list.
 *
 * For example if a mirror test is defined with a `fixture(size_t, i)`
 * argument, `MIRROR_EXTRACT(mirror_fixture, ...)` will expand to:
 *
 *     (mirror_fixture, (size_t, i))
 *
 * If no fixture parameter exists, it expands to this instead:
 *
 *     (mirror_fixtureNONE, fallback)
 */

#define MIRROR_EXTRACT(name, fallback, a, b, c, d, e, f, g, h) \
    MIRROR_EXTRACT_8(name, fallback, a, b, c, d, e, f, g, h)

#define MIRROR_EXTRACT_0(name, fallback) \
    (name##NONE, fallback)

#define MIRROR_EXTRACT_1(name, fallback, a) \
    MIRROR_EXTRACT_ARG(name, fallback, a, MIRROR_EXTRACT_0(name, fallback))

#define MIRROR_EXTRACT_2(name, fallback, a, b) \
    MIRROR_EXTRACT_ARG(name, fallback, a, MIRROR_EXTRACT_1(name, fallback, b))

#define MIRROR_EXTRACT_3(name, fallback, a, b, c) \
    MIRROR_EXTRACT_ARG(name, fallback, a, MIRROR_EXTRACT_2(name, fallback, b, c))

#define MIRROR_EXTRACT_4(name, fallback, a, b, c, d) \
    MIRROR_EXTRACT_ARG(name, fallback, a, MIRROR_EXTRACT_3(name, fallback, b, c, d))

#define MIRROR_EXTRACT_5(name, fallback, a, b, c, d, e) \
    MIRROR_EXTRACT_ARG(name, fallback, a, MIRROR_EXTRACT_4(name, fallback, b, c, d, e))

#define MIRROR_EXTRACT_6(name, fallback, a, b, c, d, e, f) \
    MIRROR_EXTRACT_ARG(name, fallback, a, MIRROR_EXTRACT_5(name, fallback, b, c, d, e, f))

#define MIRROR_EXTRACT_7(name, fallback, a, b, c, d, e, f, g) \
    MIRROR_EXTRACT_ARG(name, fallback, a, MIRROR_EXTRACT_6(name, fallback, b, c, d, e, f, g))

#define MIRROR_EXTRACT_8(name, fallback, a, b, c, d, e, f, g, h) \
    MIRROR_EXTRACT_ARG(name, fallback, a, MIRROR_EXTRACT_7(name, fallback, b, c, d, e, f, g, h))

#define MIRROR_EXTRACT_ARG(name, fallback, arg, next) \
    GHOST_EXPAND(MIRROR_EXTRACT_##name##_##arg(name, fallback, arg, next))



/*
 * Here we match up all the arguments we can extract with all of the possible
 * arguments in mirror. Each of these expands to either MATCH or NOMATCH.
 *
 * Note that the user arguments can be prefixed with mirror_. We have to handle
 * both cases.
 *
 * TODO should be able to do this in two steps to prevent duplication, where
 * first step adds/removes mirror prefix of user arg, like how we do it for
 * fixtures or info below
 */

/* unprefixed */
/* id */
#define MIRROR_EXTRACT_mirror_id_ MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_death MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_fixture(type, name) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_id(id) MIRROR_EXTRACT_MATCH /*MATCH*/
#define MIRROR_EXTRACT_mirror_id_it(desc) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_name(n) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_nothing MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_param(type, name) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_params(params) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_serial MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_setup(fn) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_suite(s) MIRROR_EXTRACT_NOMATCH /* TODO delete suite */
#define MIRROR_EXTRACT_mirror_id_suffix(s) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_teardown(fn) MIRROR_EXTRACT_NOMATCH
/* fixture */
#define MIRROR_EXTRACT_mirror_fixture_ MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_death MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_fixture(type, name) MIRROR_EXTRACT_MATCH /*MATCH*/
#define MIRROR_EXTRACT_mirror_fixture_id(id) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_it(desc) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_name(n) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_nothing MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_param(type, name) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_params(params) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_serial MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_setup(fn) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_suite(s) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_suffix(s) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_teardown(fn) MIRROR_EXTRACT_NOMATCH
/* param */
#define MIRROR_EXTRACT_mirror_param_ MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_death MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_fixture(type, name) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_id(id) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_it(desc) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_name(n) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_nothing MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_param(type, name) MIRROR_EXTRACT_MATCH /*MATCH*/
#define MIRROR_EXTRACT_mirror_param_params(params) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_serial MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_setup(fn) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_suite(s) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_suffix(s) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_teardown(fn) MIRROR_EXTRACT_NOMATCH

/* prefixed */
/* id */
#define MIRROR_EXTRACT_mirror_id_mirror_ MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_mirror_death MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_mirror_fixture(type, name) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_mirror_id(id) MIRROR_EXTRACT_MATCH /*MATCH*/
#define MIRROR_EXTRACT_mirror_id_mirror_it(desc) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_mirror_name(n) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_mirror_nothing MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_mirror_param(type, name) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_mirror_params(params) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_mirror_serial MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_mirror_setup(fn) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_mirror_suite(s) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_mirror_suffix(s) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_id_mirror_teardown(fn) MIRROR_EXTRACT_NOMATCH
/* fixture */
#define MIRROR_EXTRACT_mirror_fixture_mirror_ MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_mirror_death MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_mirror_fixture(type, name) MIRROR_EXTRACT_MATCH /*MATCH*/
#define MIRROR_EXTRACT_mirror_fixture_mirror_id(id) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_mirror_it(desc) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_mirror_name(n) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_mirror_nothing MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_mirror_param(type, name) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_mirror_params(params) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_mirror_serial MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_mirror_setup(fn) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_mirror_suite(s) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_mirror_suffix(s) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_fixture_mirror_teardown(fn) MIRROR_EXTRACT_NOMATCH
/* param */
#define MIRROR_EXTRACT_mirror_param_mirror_ MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_mirror_death MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_mirror_fixture(type, name) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_mirror_id(id) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_mirror_it(desc) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_mirror_name(n) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_mirror_nothing MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_mirror_param(type, name) MIRROR_EXTRACT_MATCH /*MATCH*/
#define MIRROR_EXTRACT_mirror_param_mirror_params(params) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_mirror_serial MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_mirror_setup(fn) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_mirror_suite(s) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_mirror_suffix(s) MIRROR_EXTRACT_NOMATCH
#define MIRROR_EXTRACT_mirror_param_mirror_teardown(fn) MIRROR_EXTRACT_NOMATCH

/* MATCH expands to the tuple containing the value.
 * NOMATCH expands to next to continue the search. */

#define MIRROR_EXTRACT_MATCH(name, fallback, arg, next) (name, MIRROR_EXTRACT_EXPAND_##arg)
#define MIRROR_EXTRACT_NOMATCH(name, fallback, arg, next) next

#define MIRROR_EXTRACT_EXPAND_fixture(type, name) (type, name)
#define MIRROR_EXTRACT_EXPAND_param(type, name) (type, name)
#define MIRROR_EXTRACT_EXPAND_id(id) id



/**
 * Pull out parts of the fixture and param tuples
 */
#if GHOST_MSVC_TRADITIONAL
    #define MIRROR_TUPLE_CAR(tuple) MIRROR_EXPAND(MIRROR_TUPLE_CAR_IMPL tuple)
    #define MIRROR_TUPLE_CDR(tuple) MIRROR_EXPAND(MIRROR_TUPLE_CDR_IMPL tuple)
#else
    #define MIRROR_TUPLE_CAR(tuple) MIRROR_TUPLE_CAR_IMPL tuple
    #define MIRROR_TUPLE_CDR(tuple) MIRROR_TUPLE_CDR_IMPL tuple
#endif
#define MIRROR_TUPLE_CAR_IMPL(a, b) a
#define MIRROR_TUPLE_CDR_IMPL(a, b) b
#define MIRROR_TUPLE_CADR(tuple) MIRROR_TUPLE_CAR(MIRROR_TUPLE_CDR(tuple))
#define MIRROR_TUPLE_CDDR(tuple) MIRROR_TUPLE_CDR(MIRROR_TUPLE_CDR(tuple))



/**
 * @def MIRROR_IMPL()
 */

#define MIRROR_IMPL(a, b, c, d, e, f, g, h) \
    MIRROR_IMPL3(\
            MIRROR_EXTRACT(mirror_id, GHOST_CONCAT(MIRROR_KEY, GHOST_COUNTER), a, b, c, d, e, f, g, h), \
            MIRROR_EXTRACT(mirror_fixture, (mirror_nothing, mirror_nothing), a, b, c, d, e, f, g, h), \
            MIRROR_EXTRACT(mirror_param, (mirror_nothing, mirror_nothing), a, b, c, d, e, f, g, h), \
            a, b, c, d, e, f, g, h)

/* PCC needs us to forward here to pre-emptively expand args (1.2 development
 * version circa 2021/2022.) Not sure what's going on yet. */
#ifdef __PCC__
    #define MIRROR_IMPL2(id_tuple, fixture_tuple, param_tuple, a, b, c, d, e, f, g, h) \
        MIRROR_IMPL3(id_tuple, fixture_tuple, param_tuple, a, b, c, d, e, f, g, h)
#else
    #define MIRROR_IMPL2 MIRROR_IMPL3
#endif

#define MIRROR_IMPL3(id_tuple, fixture_tuple, param_tuple, a, b, c, d, e, f, g, h) \
    MIRROR_IMPL4( \
        MIRROR_TUPLE_CDR(id_tuple), \
        MIRROR_TUPLE_CAR(fixture_tuple), MIRROR_TUPLE_CADR(fixture_tuple), MIRROR_TUPLE_CDDR(fixture_tuple), \
        MIRROR_TUPLE_CAR(param_tuple), MIRROR_TUPLE_CADR(param_tuple), MIRROR_TUPLE_CDDR(param_tuple), \
        a, b, c, d, e, f, g, h)

/* forward to expand out the args. */
#define MIRROR_IMPL4( \
        id, \
        is_fixture, fixture_type, fixture_name, \
        is_param, param_type, param_name, \
        a, b, c, d, e, f, g, h) \
    MIRROR_IMPL5( \
            id, \
            is_fixture, fixture_type, fixture_name, \
            is_param, param_type, param_name, \
            a, b, c, d, e, f, g, h)

#define MIRROR_IMPL5( \
        id, \
        is_fixture, fixture_type, fixture_name, \
        is_param, param_type, param_name, \
        a, b, c, d, e, f, g, h) \
    MIRROR_IMPL_##is_fixture##_##is_param(\
            id, fixture_type, fixture_name, param_type, param_name, a, b, c, d, e, f, g, h)



/*
 * MIRROR_IMPL_*() variants
 *
 * This is where we finally define the test function. The definitions differ
 * based on whether we have fixture or param arguments.
 *
 * All of them forward the parameters to MIRROR_IMPL_REGISTER() to perform the
 * registration.
 */

#define MIRROR_IMPL_mirror_fixtureNONE_mirror_paramNONE( \
        id, fixture_type, fixture_name, param_type, param_name, a, b, c, d, e, f, g, h) \
    \
    /* declare user test function */ \
    static void GHOST_CONCAT(mirror_TEST_, id)(void); \
    \
    /* declare thunk */ \
    static void GHOST_CONCAT(mirror_THUNK_, id)(void* vfixture, void* vparam) { \
        ghost_discard(vfixture); \
        ghost_discard(vparam); \
        GHOST_CONCAT(mirror_TEST_, id)(); \
    } \
    \
    /* declare and register test */ \
    MIRROR_IMPL_REGISTER(id, a, b, c, d, e, f, g, h) \
    \
    /* open test */ \
    static void GHOST_CONCAT(mirror_TEST_, id)(void)

#define MIRROR_IMPL_mirror_fixture_mirror_paramNONE( \
        id, fixture_type, fixture_name, param_type, param_name, a, b, c, d, e, f, g, h) \
    \
    /* declare user test function */ \
    static void GHOST_CONCAT(mirror_TEST_, id)(fixture_type fixture_name); \
    \
    /* declare test thunk */ \
    static void GHOST_CONCAT(mirror_THUNK_, id)(void* vfixture, void* vparam) { \
        ghost_discard(vparam); \
        typedef fixture_type GHOST_CONCAT(mirror_FIXTURE_type_, id); \
        GHOST_CONCAT(mirror_TEST_, id)( \
                *ghost_static_cast(GHOST_CONCAT(mirror_FIXTURE_type_, id)*, vfixture) \
            ); \
    } \
    \
    /* declare fixture thunks (if any) */ \
    MIRROR_IMPL_DECLARE_FIXTURE_THUNKS(id, fixture_type, a, b, c, d, e, f, g, h) \
    \
    /* declare and register test */ \
    MIRROR_IMPL_REGISTER(id, a, b, c, d, e, f, g, h) \
    \
    /* open test */ \
    static void GHOST_CONCAT(mirror_TEST_, id)(fixture_type fixture_name)

#define MIRROR_IMPL_mirror_fixtureNONE_mirror_param( \
        id, fixture_type, fixture_name, param_type, param_name, a, b, c, d, e, f, g, h) \
    \
    /* declare user test function */ \
    static void GHOST_CONCAT(mirror_TEST_, id)(param_type param_name); \
    \
    /* declare thunk */ \
    static void GHOST_CONCAT(mirror_THUNK_, id)(void* vfixture, void* vparam) { \
        ghost_discard(vfixture); \
        typedef param_type GHOST_CONCAT(mirror_PARAM_type_, id); \
        GHOST_CONCAT(mirror_TEST_, id)( \
                *ghost_static_cast(GHOST_CONCAT(mirror_PARAM_type_, id)*, vparam) \
            ); \
    } \
    \
    /* declare and register test */ \
    MIRROR_IMPL_REGISTER(id, a, b, c, d, e, f, g, h) \
    \
    /* open test */ \
    static void GHOST_CONCAT(mirror_TEST_, id)(param_type param_name)

#define MIRROR_IMPL_mirror_fixture_mirror_param( \
        id, fixture_type, fixture_name, param_type, param_name, a, b, c, d, e, f, g, h) \
    \
    /* declare user test function */ \
    static void GHOST_CONCAT(mirror_TEST_, id)(fixture_type fixture_name, param_type param_name); \
    \
    /* declare thunk */ \
    static void GHOST_CONCAT(mirror_THUNK_, id)(void* vfixture, void* vparam) { \
        typedef fixture_type GHOST_CONCAT(mirror_FIXTURE_type_, id); \
        typedef param_type GHOST_CONCAT(mirror_PARAM_type_, id); \
        GHOST_CONCAT(mirror_TEST_, id)( \
                *ghost_static_cast(GHOST_CONCAT(mirror_FIXTURE_type_, id)*, vfixture), \
                *ghost_static_cast(GHOST_CONCAT(mirror_PARAM_type_, id)*, vparam) \
            ); \
    } \
    \
    /* declare and register test */ \
    MIRROR_IMPL_REGISTER(id, a, b, c, d, e, f, g, h) \
    \
    /* open test */ \
    static void GHOST_CONCAT(mirror_TEST_, id)(fixture_type fixture_name)



/*
 * This is where a test is registered.
 */

/* For some reason setting id doesn't work on PCC 1.1.0, figure it out later */
#ifdef __PCC__
    #if __PCC__ == 1 && __PCC_MINOR__ == 1
        #define MIRROR_IMPL_SET_ID(testid) /*nothing*/
    #endif
#endif
#ifndef MIRROR_IMPL_SET_ID
    #define MIRROR_IMPL_SET_ID(testid) test.id = GHOST_STRINGIFY(testid);
#endif

#define MIRROR_IMPL_REGISTER(testid, a, b, c, d, e, f, g, h) \
    MIRROR_REGISTRATION_BLOCK(testid) { \
        \
        /* declare test case info */ \
        static mirror_test_t test; \
        MIRROR_IMPL_SET_ID(testid) \
        test.fn = GHOST_CONCAT(mirror_THUNK_, testid); \
        test.file = __FILE__; \
        test.line = __LINE__; \
        test.name = MIRROR_NAME; \
        /*printf("registering %i %s\n",testid, test.name);*/ \
        \
        /* generate optional info */ \
        MIRROR_IMPL_TEST_INFO(testid, a, b, c, d, e, f, g, h) \
        \
        /* register the test case */ \
        mirror_register_test(&test); \
    } \



/*
 * Generate test info
 */

/* TODO more cparser hacks */
#ifdef __CPARSER__
    #define MIRROR_IMPL_TEST_INFO(id, a, b, c, d, e, f, g, h) /*nothing*/
#else
    #define MIRROR_IMPL_TEST_INFO(id, a, b, c, d, e, f, g, h) \
        GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_TEST_INFO_##a)(id, MIRROR_IMPL_TEST_INFO_OPTIONS_##a)) \
        MIRROR_IMPL_TEST_INFO_7(id, b, c, d, e, f, g, h)
#endif

#define MIRROR_IMPL_TEST_INFO_7(id, a, b, c, d, e, f, g) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_TEST_INFO_##a)(id, MIRROR_IMPL_TEST_INFO_OPTIONS_##a)) \
    MIRROR_IMPL_TEST_INFO_6(id, b, c, d, e, f, g)

#define MIRROR_IMPL_TEST_INFO_6(id, a, b, c, d, e, f) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_TEST_INFO_##a)(id, MIRROR_IMPL_TEST_INFO_OPTIONS_##a)) \
    MIRROR_IMPL_TEST_INFO_5(id, b, c, d, e, f)

#define MIRROR_IMPL_TEST_INFO_5(id, a, b, c, d, e) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_TEST_INFO_##a)(id, MIRROR_IMPL_TEST_INFO_OPTIONS_##a)) \
    MIRROR_IMPL_TEST_INFO_4(id, b, c, d, e)

#define MIRROR_IMPL_TEST_INFO_4(id, a, b, c, d) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_TEST_INFO_##a)(id, MIRROR_IMPL_TEST_INFO_OPTIONS_##a)) \
    MIRROR_IMPL_TEST_INFO_3(id, b, c, d)

#define MIRROR_IMPL_TEST_INFO_3(id, a, b, c) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_TEST_INFO_##a)(id, MIRROR_IMPL_TEST_INFO_OPTIONS_##a)) \
    MIRROR_IMPL_TEST_INFO_2(id, b, c)

#define MIRROR_IMPL_TEST_INFO_2(id, a, b) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_TEST_INFO_##a)(id, MIRROR_IMPL_TEST_INFO_OPTIONS_##a)) \
    MIRROR_IMPL_TEST_INFO_1(id, b)

#define MIRROR_IMPL_TEST_INFO_1(id, a) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_TEST_INFO_##a)(id, MIRROR_IMPL_TEST_INFO_OPTIONS_##a))

/* forward mirror-prefixed args */
#define MIRROR_IMPL_TEST_INFO_mirror_id MIRROR_IMPL_TEST_INFO_id
#define MIRROR_IMPL_TEST_INFO_mirror_it MIRROR_IMPL_TEST_INFO_it
#define MIRROR_IMPL_TEST_INFO_mirror_fixture MIRROR_IMPL_TEST_INFO_fixture
#define MIRROR_IMPL_TEST_INFO_mirror_death MIRROR_IMPL_TEST_INFO_death
#define MIRROR_IMPL_TEST_INFO_mirror_setup MIRROR_IMPL_TEST_INFO_setup
#define MIRROR_IMPL_TEST_INFO_mirror_teardown MIRROR_IMPL_TEST_INFO_teardown
#define MIRROR_IMPL_TEST_INFO_mirror_name MIRROR_IMPL_TEST_INFO_name
#define MIRROR_IMPL_TEST_INFO_mirror_nothing MIRROR_IMPL_TEST_INFO_nothing

/* forward mirror-prefixed arg options */
#define MIRROR_IMPL_TEST_INFO_OPTIONS_mirror_id MIRROR_IMPL_TEST_INFO_OPTIONS_id
#define MIRROR_IMPL_TEST_INFO_OPTIONS_mirror_it MIRROR_IMPL_TEST_INFO_OPTIONS_it
#define MIRROR_IMPL_TEST_INFO_OPTIONS_mirror_fixture MIRROR_IMPL_TEST_INFO_OPTIONS_fixture
#define MIRROR_IMPL_TEST_INFO_OPTIONS_mirror_death MIRROR_IMPL_TEST_INFO_OPTIONS_death
#define MIRROR_IMPL_TEST_INFO_OPTIONS_mirror_setup MIRROR_IMPL_TEST_INFO_OPTIONS_setup
#define MIRROR_IMPL_TEST_INFO_OPTIONS_mirror_teardown MIRROR_IMPL_TEST_INFO_OPTIONS_teardown
#define MIRROR_IMPL_TEST_INFO_OPTIONS_mirror_nothing MIRROR_IMPL_TEST_INFO_OPTIONS_nothing
#define MIRROR_IMPL_TEST_INFO_OPTIONS_mirror_name MIRROR_IMPL_TEST_INFO_OPTIONS_name

/* unused options */
#define MIRROR_IMPL_TEST_INFO_ MIRROR_EAT_2
#define MIRROR_IMPL_TEST_INFO_nothing MIRROR_EAT_2
#define MIRROR_IMPL_TEST_INFO_id(id) MIRROR_EAT_2

/* info follows */

#define MIRROR_IMPL_TEST_INFO_death(id, junk) test.death = ghost_true;

#define MIRROR_IMPL_TEST_INFO_OPTIONS_fixture(type, name) type, name
#define MIRROR_IMPL_TEST_INFO_fixture(type, name) MIRROR_IMPL_TEST_INFO_fixture_2
#define MIRROR_IMPL_TEST_INFO_fixture_2(id, type, name) test.fixture_size = sizeof(type);

#define MIRROR_IMPL_TEST_INFO_OPTIONS_name(fn) fn
#define MIRROR_IMPL_TEST_INFO_name(fn) MIRROR_IMPL_TEST_INFO_name_2
#define MIRROR_IMPL_TEST_INFO_name_2(id, n) test.name = n;

#define MIRROR_IMPL_TEST_INFO_OPTIONS_setup(fn) fn
#define MIRROR_IMPL_TEST_INFO_setup(fn) MIRROR_IMPL_TEST_INFO_setup_2
#define MIRROR_IMPL_TEST_INFO_setup_2(id, fn) test.fixture_setup = GHOST_CONCAT(mirror_SETUP_THUNK_, id);

#define MIRROR_IMPL_TEST_INFO_OPTIONS_teardown(fn) fn
#define MIRROR_IMPL_TEST_INFO_teardown(fn) MIRROR_IMPL_TEST_INFO_teardown_2
#define MIRROR_IMPL_TEST_INFO_teardown_2(id, fn) test.fixture_teardown = GHOST_CONCAT(mirror_TEARDOWN_THUNK_, id);

#define MIRROR_IMPL_TEST_INFO_OPTIONS_it(fn) fn
#define MIRROR_IMPL_TEST_INFO_it(fn) MIRROR_IMPL_TEST_INFO_it_2
#define MIRROR_IMPL_TEST_INFO_it_2(id, it) test.description = it;



/*
 * Declare fixture thunks
 */

#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS(id, fixture_type, a, b, c, d, e, f, g, h) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_##a)(id, fixture_type, MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_##a)) \
    MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_7(id, fixture_type, b, c, d, e, f, g, h) \

#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_7(id, fixture_type, a, b, c, d, e, f, g) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_##a)(id, fixture_type, MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_##a)) \
    MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_6(id, fixture_type, b, c, d, e, f, g) \

#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_6(id, fixture_type, a, b, c, d, e, f) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_##a)(id, fixture_type, MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_##a)) \
    MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_5(id, fixture_type, b, c, d, e, f) \

#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_5(id, fixture_type, a, b, c, d, e) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_##a)(id, fixture_type, MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_##a)) \
    MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_4(id, fixture_type, b, c, d, e) \

#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_4(id, fixture_type, a, b, c, d) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_##a)(id, fixture_type, MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_##a)) \
    MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_3(id, fixture_type, b, c, d) \

#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_3(id, fixture_type, a, b, c) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_##a)(id, fixture_type, MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_##a)) \
    MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_2(id, fixture_type, b, c) \

#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_2(id, fixture_type, a, b) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_##a)(id, fixture_type, MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_##a)) \
    MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_1(id, fixture_type, b) \

#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_1(id, fixture_type, a) \
    GHOST_EXPAND(GHOST_DEFER(MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_##a)(id, fixture_type, MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_##a)) \

/* forward mirror-prefixed args */
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_mirror_id MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_id
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_mirror_name MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_name
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_mirror_it MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_it
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_mirror_fixture MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_fixture
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_mirror_death MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_death
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_mirror_setup MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_setup
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_mirror_teardown MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_teardown
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_mirror_nothing MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_nothing

/* forward mirror-prefixed arg options (that we care about) */
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_mirror_setup MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_setup
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_mirror_teardown MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_teardown

/* all the stuff that doesn't involve fixture thunks */
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_ MIRROR_EAT_3
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_death MIRROR_EAT_3
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_fixture(type, name) MIRROR_EAT_3
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_id(id) MIRROR_EAT_3
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_it(id) MIRROR_EAT_3
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_name(name) MIRROR_EAT_3
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_nothing MIRROR_EAT_3

/* our actual fixture thunks */

#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_setup(fn) fn
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_setup(fn) MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_setup_2
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_setup_2(id, fixture_type, fn) \
    static void GHOST_CONCAT(mirror_SETUP_THUNK_, id)(void* vfixture) { \
        typedef fixture_type GHOST_CONCAT(mirror_FIXTURE_type_, id); \
        *ghost_static_cast(GHOST_CONCAT(mirror_FIXTURE_type_, id)*, vfixture) = fn(); \
    }

#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_OPTIONS_teardown(fn) fn
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_teardown(fn) MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_teardown_2
#define MIRROR_IMPL_DECLARE_FIXTURE_THUNKS_teardown_2(id, fixture_type, fn) \
    static void GHOST_CONCAT(mirror_TEARDOWN_THUNK_, id)(void* vfixture) { \
        typedef fixture_type GHOST_CONCAT(mirror_FIXTURE_type_, id); \
        fn(*ghost_static_cast(GHOST_CONCAT(mirror_FIXTURE_type_, id)*, vfixture)); \
    }






#ifdef __cplusplus
}
#endif

#endif
