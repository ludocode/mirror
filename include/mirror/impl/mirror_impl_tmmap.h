/*
 * MIT No Attribution
 *
 * Copyright (c) 2022 Fraser Heavy Software
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef MIRROR_IMPL_TMMAP
#define MIRROR_IMPL_TMMAP

#include "mirror/impl/mirror_impl_iwbt.h"

/* TODO move to mirror_impl_ghost.h */
#include "ghost/debug/ghost_assert.h"
#include "ghost/ghost_core.h"
#include "ghost/impl/ghost_impl_function_wrapper.h"
#include "ghost/language/ghost_container_of.h"
#include "ghost/language/ghost_header_function.h"
#include "ghost/language/ghost_header_inline.h"
#include "ghost/language/ghost_inline_opt.h"
#include "ghost/language/ghost_maybe_unused.h"
#include "ghost/language/ghost_null.h"
#include "ghost/language/ghost_unreachable.h"
#include "ghost/type/bool/ghost_bool.h"
#include "ghost/type/bool/ghost_false.h"
#include "ghost/type/bool/ghost_true.h"
#include "ghost/type/size_t/ghost_size_t.h"

/*
 * This is a templated multimap backed by a type-erased weight-balanced tree.
 * It was originally written for Ghost but it has been pulled into Mirror
 * temporarily while I sort out what to do with it.
 */

#ifndef MIRROR_IMPL_TMMAP_DOCUMENTATION
#define MIRROR_IMPL_TMMAP_DOCUMENTATION 0
#endif

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_ANY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that returns any element in the map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_ANY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that returns any element in the map.
 */

/**
 * Returns any value in the map or null if the map is empty.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
value_t* mirror_tmmap_any(mirror_tmmap_t* map);
#endif

        #define MIRROR_TMMAP_DECLARE_ANY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            value_t* prefix##_any(prefix##_t* map) { \
                mirror_iwbt_node_t* node; \
                ghost_assert(map != ghost_null, ""); \
                node = mirror_iwbt_root(&map->tree); \
                return (node == ghost_null) ? ghost_null : ghost_container_of(node, value_t, node_field); \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_ANY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_CLEAR(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that clears the map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_CLEAR(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that clears the map.
 */

/**
 * Clears the map.
 *
 * This can be used to initialize a map. It is equivalent to zeroing.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
void mirror_tmmap_clear(mirror_tmmap_t* map);
#endif

        #define MIRROR_TMMAP_DECLARE_CLEAR(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            void prefix##_clear(prefix##_t* map) { \
                ghost_assert(map != ghost_null, ""); \
                mirror_iwbt_clear(&map->tree); \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_CLEAR(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_COUNT(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that returns the number of elements in a map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_COUNT(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that returns the number of elements in a map.
 */

/**
 * Returns the number of values in the map.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
size_t mirror_tmmap_count(mirror_tmmap_t* map);
#endif

        #define MIRROR_TMMAP_DECLARE_COUNT(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            ghost_size_t prefix##_count(prefix##_t* map) { \
                ghost_assert(map != ghost_null, ""); \
                return mirror_iwbt_count(&map->tree); \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_COUNT(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_FIND_AFTER(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Declares a function that finds a map value by key, or the value before it if
 * it were in the map.
 */
/* TODO rename all find_after() to something better, like find_last_or_after(), find_last_or_next() */

/**
 * @def MIRROR_TMMAP_DEFINE_FIND_AFTER(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Defines a function that finds a map value by key, or the value after it if
 * it were in the map.
 */

/**
 * Finds the last value with the given key, or if no matching value exists,
 * finds the value that would be immediately following it if one were in the
 * map.
 *
 * If no such value exists (i.e. the tree is empty or all values have keys
 * smaller than the given key), null is returned.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 *
 * @param found If not null, this is set to true if the value's key is equal
 *              to the given key (i.e. a value with a matching key was found)
 *              and false otherwise.
 */
value_t* mirror_tmmap_find_after(prefix##_t* map, key_t key, bool* /*nullable*/ found);
#endif

        #define MIRROR_TMMAP_DECLARE_FIND_AFTER(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            noninline_attrib \
            value_t* prefix##_find_after(prefix##_t* map, key_t key, ghost_bool* /*nullable*/ equal); \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_FIND_AFTER(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            \
            noninline_attrib \
            value_t* prefix##_find_after(prefix##_t* map, key_t key, ghost_bool* /*nullable*/ equal) { \
                mirror_iwbt_node_t* node; \
                mirror_iwbt_node_t* next_node; \
                \
                mirror_iwbt_log(("find before\n")); \
                ghost_assert(map != ghost_null, ""); \
                \
                if (equal != ghost_null) \
                    *equal = ghost_false; \
                \
                node = mirror_iwbt_root(&map->tree); \
                if (node == ghost_null) \
                    return ghost_null; \
                \
                /* step into the tree until we find a match. */ \
                for (;;) { \
                    value_t* value = ghost_container_of(node, value_t, node_field); \
                    int cmp = compare_fn(key, value_key_fn(value)); \
                    mirror_iwbt_log(("cmp: %i\n", cmp)); \
                    if (cmp > 0) { \
                        next_node = mirror_iwbt_node_last_child(node); \
                        if (next_node == ghost_null) { \
                            node = mirror_iwbt_next(&map->tree, node); \
                            return (node == ghost_null) ? ghost_null : \
                                    ghost_container_of(node, value_t, node_field); \
                        } \
                        node = next_node; \
                    } else if (cmp < 0) { \
                        next_node = mirror_iwbt_node_first_child(node); \
                        if (next_node == ghost_null) \
                            return value; \
                        node = next_node; \
                    } else { \
                        /* found a match. keep searching right for the last match. */ \
                        value_t* match = value; \
                        *equal = ghost_true; \
                        node = mirror_iwbt_node_last_child(node); \
                        for (;;) { \
                            if (node == ghost_null) \
                                return match; \
                            value = ghost_container_of(node, value_t, node_field); \
                            cmp = compare_fn(key, value_key_fn(value)); \
                            if (cmp > 0) { \
                                /* we're right of the first match. this key can't possibly be smaller. */ \
                                ghost_unreachable(ghost_null); \
                            } else if (cmp < 0) { \
                                node = mirror_iwbt_node_first_child(node); \
                            } else { \
                                match = value; \
                                node = mirror_iwbt_node_last_child(node); \
                            } \
                        } \
                    } \
                } \
                \
                ghost_unreachable(ghost_null); \
            } \
            \
            GHOST_IMPL_FUNCTION_CLOSE

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_FIND_BEFORE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Declares a function that finds a map value by key, or the value before it if
 * it were in the map.
 */
/* TODO rename all find_before() to something better, like find_first_or_before(), find_first_or_previous() */

/**
 * @def MIRROR_TMMAP_DEFINE_FIND_BEFORE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Defines a function that finds a map value by key, or the value before it if
 * it were in the map.
 */

/**
 * Finds the first value with the given key, or if no matching value exists,
 * finds the value that would be immediately preceding it if one were in the
 * map.
 *
 * If no such value exists (i.e. the tree is empty or all values have keys
 * larger than the given key), null is returned.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 *
 * @param found If not null, this is set to true if the value's key is equal
 *              to the given key (i.e. a value with a matching key was found)
 *              and false otherwise.
 */
value_t* mirror_tmmap_find_before(prefix##_t* map, key_t key, bool* /*nullable*/ found);
#endif

        #define MIRROR_TMMAP_DECLARE_FIND_BEFORE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            noninline_attrib \
            value_t* prefix##_find_before(prefix##_t* map, key_t key, ghost_bool* /*nullable*/ equal); \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_FIND_BEFORE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            \
            noninline_attrib \
            value_t* prefix##_find_before(prefix##_t* map, key_t key, ghost_bool* /*nullable*/ equal) { \
                mirror_iwbt_node_t* node; \
                mirror_iwbt_node_t* next_node; \
                \
                mirror_iwbt_log(("find before\n")); \
                ghost_assert(map != ghost_null, ""); \
                \
                if (equal != ghost_null) \
                    *equal = ghost_false; \
                \
                node = mirror_iwbt_root(&map->tree); \
                if (node == ghost_null) \
                    return ghost_null; \
                \
                /* step into the tree until we find a match. */ \
                for (;;) { \
                    value_t* value = ghost_container_of(node, value_t, node_field); \
                    int cmp = compare_fn(key, value_key_fn(value)); \
                    mirror_iwbt_log(("cmp: %i\n", cmp)); \
                    if (cmp > 0) { \
                        next_node = mirror_iwbt_node_last_child(node); \
                        if (next_node == ghost_null) \
                            return value; \
                        node = next_node; \
                    } else if (cmp < 0) { \
                        next_node = mirror_iwbt_node_first_child(node); \
                        if (next_node == ghost_null) { \
                            node = mirror_iwbt_previous(&map->tree, node); \
                            return (node == ghost_null) ? ghost_null : \
                                    ghost_container_of(node, value_t, node_field); \
                        } \
                        node = next_node; \
                    } else { \
                        /* found a match. keep searching left for the first match. */ \
                        value_t* match = value; \
                        *equal = ghost_true; \
                        node = mirror_iwbt_node_first_child(node); \
                        for (;;) { \
                            if (node == ghost_null) \
                                return match; \
                            value = ghost_container_of(node, value_t, node_field); \
                            cmp = compare_fn(key, value_key_fn(value)); \
                            if (cmp > 0) { \
                                node = mirror_iwbt_node_last_child(node); \
                            } else if (cmp < 0) { \
                                /* we're left of the first match. this key can't possibly be greater. */ \
                                ghost_unreachable(ghost_null); \
                            } else { \
                                match = value; \
                                node = mirror_iwbt_node_first_child(node); \
                            } \
                        } \
                    } \
                } \
                \
                ghost_unreachable(ghost_null); \
            } \
            \
            GHOST_IMPL_FUNCTION_CLOSE

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_FIND_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Declares a function that finds a map value by key.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_FIND_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Defines a function that finds a map value by key.
 */

/**
 * Finds the first value in the map with the given key or returns null if no
 * such value exists.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
value_t* mirror_tmmap_find_first(mirror_tmmap_t* map, key_t key);
#endif

        #define MIRROR_TMMAP_DECLARE_FIND_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            value_t* prefix##_find_first(prefix##_t* map, key_t key) { \
                ghost_bool equal; \
                value_t* value = prefix##_find_before(map, key, &equal); \
                return equal ? value : ghost_null; \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_FIND_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_FIND_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Declares a function that finds a map value by key.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_FIND_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Defines a function that finds a map value by key.
 */

/**
 * Finds the last value in the map with the given key or returns null if no
 * such value exists.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
value_t* mirror_tmmap_find_last(mirror_tmmap_t* map, key_t key);
#endif

        #define MIRROR_TMMAP_DECLARE_FIND_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            value_t* prefix##_find_last(prefix##_t* map, key_t key) { \
                ghost_bool equal; \
                value_t* value = prefix##_find_after(map, key, &equal); \
                return equal ? value : ghost_null; \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_FIND_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that returns the first element in the map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that returns the first element in the map.
 */

/**
 * Returns the first value in the map or null if the map is empty.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
value_t* mirror_tmmap_first(mirror_tmmap_t* map);
#endif

        #define MIRROR_TMMAP_DECLARE_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            value_t* prefix##_first(prefix##_t* map) { \
                mirror_iwbt_node_t* node; \
                ghost_assert(map != ghost_null, ""); \
                node = mirror_iwbt_first(&map->tree); \
                return (node == ghost_null) ? ghost_null : ghost_container_of(node, value_t, node_field); \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/



/* Declare functions */
#ifdef GHOST_IMPL_TMMAP_DECLARE_FUNCTIONS
    #error
#endif
#define GHOST_IMPL_TMMAP_DECLARE_FUNCTIONS(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_ANY                 (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_CLEAR               (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_COUNT               (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_FIND_AFTER          (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_FIND_BEFORE         (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_FIND_FIRST          (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_FIND_LAST           (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_FIRST               (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_INSERT_AFTER        (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_INSERT_BEFORE       (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_INSERT_FIRST        (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_INSERT_LAST         (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_IS_EMPTY            (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_LAST                (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_NEXT                (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_NEXT_MATCH          (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_PREVIOUS            (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_PREVIOUS_MATCH      (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_REMOVE              (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_REMOVE_ALL_WITH_KEY (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_REMOVE_ANY          (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_REMOVE_FIRST        (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_REMOVE_LAST         (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
    MIRROR_TMMAP_DECLARE_SWAP                (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)

/* Define functions */
#ifdef GHOST_IMPL_TMMAP_DEFINE_FUNCTIONS
    #error
#endif
#define GHOST_IMPL_TMMAP_DEFINE_FUNCTIONS(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_ANY                 (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_CLEAR               (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_COUNT               (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_FIND_AFTER          (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_FIND_BEFORE         (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_FIND_FIRST          (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_FIND_LAST           (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_FIRST               (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_INSERT_AFTER        (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_INSERT_BEFORE       (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_INSERT_FIRST        (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_INSERT_LAST         (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_IS_EMPTY            (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_LAST                (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_NEXT                (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_NEXT_MATCH          (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_PREVIOUS            (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_PREVIOUS_MATCH      (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_REMOVE              (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_REMOVE_ALL_WITH_KEY (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_REMOVE_ANY          (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_REMOVE_FIRST        (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_REMOVE_LAST         (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
    MIRROR_TMMAP_DEFINE_SWAP                (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib)

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_INSERT_AFTER(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Declares a function that inserts a value into a map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_INSERT_AFTER(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Defines a function that inserts a value into a map.
 */

/**
 * Inserts the given value after the given reference value with a matching
 * key into the list of values for this key.
 *
 * The keys of the given values must match.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 *
 * @return The previous value or null
 */
void mirror_tmmap_insert_after(mirror_tmmap_t* map, value_t* reference_value, value_t* value_to_insert);
#endif

        #define MIRROR_TMMAP_DECLARE_INSERT_AFTER(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            void prefix##_insert_after(prefix##_t* map, value_t* reference_value, value_t* value_to_insert) { \
                ghost_assert(0 == compare_fn(value_key_fn(value_to_insert), value_key_fn(reference_value)), \
                        "cannot insert after value with non-matching key"); \
                mirror_iwbt_insert_after(&map->tree, &reference_value->node_field, &value_to_insert->node_field); \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_INSERT_AFTER(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /* nothing */

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_INSERT_BEFORE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Declares a function that inserts a value into a map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_INSERT_BEFORE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Defines a function that inserts a value into a map.
 */

/**
 * Inserts the given value before the given reference value with a matching
 * key into the list of values for this key.
 *
 * The keys of the given values must match.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 *
 * @return The previous value or null
 */
void mirror_tmmap_insert_before(mirror_tmmap_t* map, value_t* reference_value, value_t* value_to_insert);
#endif

        #define MIRROR_TMMAP_DECLARE_INSERT_BEFORE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            void prefix##_insert_before(prefix##_t* map, value_t* reference_value, value_t* value_to_insert) { \
                ghost_assert(0 == compare_fn(value_key_fn(value_to_insert), value_key_fn(reference_value)), \
                        "cannot insert before value with non-matching key"); \
                mirror_iwbt_insert_before(&map->tree, &reference_value->node_field, &value_to_insert->node_field); \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_INSERT_BEFORE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /* nothing */

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_INSERT_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Declares a function that inserts a value into a map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_INSERT_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Defines a function that inserts a value into a map.
 */

/**
 * Inserts the given value as the first value in the list of values for this
 * key.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 *
 * @return The previous value or null
 */
void mirror_tmmap_insert_first(mirror_tmmap_t* map, value_t* value);
#endif

        #define MIRROR_TMMAP_DECLARE_INSERT_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            noninline_attrib \
            void prefix##_insert_first(prefix##_t* map, value_t* value); \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_INSERT_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            \
            noninline_attrib \
            void prefix##_insert_first(prefix##_t* map, value_t* value) { \
                ghost_bool equal; \
                value_t* before; \
                ghost_assert(map != ghost_null, ""); \
                ghost_assert(value != ghost_null, ""); \
                \
                before = prefix##_find_before(map, value_key_fn(value), &equal); \
                if (equal) \
                    before = prefix##_previous(map, before); \
                \
                mirror_iwbt_insert_after(&map->tree, \
                        (before == ghost_null) ? ghost_null : &before->node_field, \
                        &value->node_field); \
            } \
            \
            GHOST_IMPL_FUNCTION_CLOSE

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_INSERT_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Declares a function that inserts a value into a map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_INSERT_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 * Defines a function that inserts a value into a map.
 */

/**
 * Inserts the given value as the last value in the list of values for this
 * key.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 *
 * @return The previous value or null
 */
void mirror_tmmap_insert_last(mirror_tmmap_t* map, value_t* value);
#endif

        #define MIRROR_TMMAP_DECLARE_INSERT_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            noninline_attrib \
            void prefix##_insert_last(prefix##_t* map, value_t* value); \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_INSERT_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            \
            noninline_attrib \
            void prefix##_insert_last(prefix##_t* map, value_t* value) { \
                ghost_bool equal; \
                value_t* after; \
                ghost_assert(map != ghost_null, ""); \
                ghost_assert(value != ghost_null, ""); \
                \
                after = prefix##_find_after(map, value_key_fn(value), &equal); \
                if (equal) \
                    after = prefix##_next(map, after); \
                \
                mirror_iwbt_insert_before(&map->tree, \
                        (after == ghost_null) ? ghost_null : &after->node_field, \
                        &value->node_field); \
            } \
            \
            GHOST_IMPL_FUNCTION_CLOSE

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_IS_EMPTY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that returns true if the map is empty.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_IS_EMPTY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that returns true if the map is empty.
 */

/**
 * Returns true if the map is empty and false otherwise.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
bool mirror_tmmap_is_empty(mirror_tmmap_t* map);
#endif

        #define MIRROR_TMMAP_DECLARE_IS_EMPTY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            ghost_bool prefix##_is_empty(prefix##_t* map) { \
                ghost_assert(map != ghost_null, ""); \
                return mirror_iwbt_is_empty(&map->tree); \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_IS_EMPTY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that returns the last element in the map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that returns the last element in the map.
 */

/**
 * Returns the last value in the map or null if the map is empty.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
value_t* mirror_tmmap_last(mirror_tmmap_t* map);
#endif

        #define MIRROR_TMMAP_DECLARE_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            value_t* prefix##_last(prefix##_t* map) { \
                mirror_iwbt_node_t* node; \
                ghost_assert(map != ghost_null, ""); \
                node = mirror_iwbt_last(&map->tree); \
                return (node == ghost_null) ? ghost_null : ghost_container_of(node, value_t, node_field); \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_NEXT(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that returns the next element in the map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_NEXT(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that returns the next element in the map.
 */

/**
 * Returns the value following the given value or null if the given value is
 * the last value in the map.
 *
 * The returned value may or may not have the same key as the given value. If
 * you want to iterate only over values with this key, call
 * mirror_tmmap_next_match().
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
value_t* mirror_tmmap_next(mirror_tmmap_t* map, value_t* value);
#endif

        #define MIRROR_TMMAP_DECLARE_NEXT(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            value_t* prefix##_next(prefix##_t* map, value_t* value) { \
                mirror_iwbt_node_t* node; \
                ghost_assert(map != ghost_null, ""); \
                node = mirror_iwbt_next(&map->tree, &value->node_field); \
                return (node == ghost_null) ? ghost_null : ghost_container_of(node, value_t, node_field); \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_NEXT(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_NEXT_MATCH(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that returns the next element in the map.
 */
/* TODO get rid of all these docs, prefix all with IMPL */

/**
 * @def MIRROR_TMMAP_DEFINE_NEXT_MATCH(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that returns the next element in the map.
 */

/**
 * Returns the next value with a matching key or null if this is the last value
 * with this key.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
value_t* mirror_tmmap_next_match(mirror_tmmap_t* map, value_t* value);
#endif

        #define MIRROR_TMMAP_DECLARE_NEXT_MATCH(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            value_t* prefix##_next_match(prefix##_t* map, value_t* value) { \
                mirror_iwbt_node_t* next_node; \
                value_t* next_value; \
                ghost_assert(map != ghost_null, ""); \
                next_node = mirror_iwbt_next(&map->tree, &value->node_field); \
                if (next_node == ghost_null) \
                    return ghost_null; \
                next_value = ghost_container_of(next_node, value_t, node_field); \
                if (0 != compare_fn(value_key_fn(value), value_key_fn(next_value))) \
                    return ghost_null; \
                return next_value; \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_NEXT_MATCH(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_PREVIOUS(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that returns the previous element in the map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_PREVIOUS(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that returns the previous element in the map.
 */

/**
 * Returns the value preceding the given value or null if the given value is
 * the first value in the map.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
value_t* mirror_tmmap_previous(mirror_tmmap_t* map, value_t* value);
#endif

        #define MIRROR_TMMAP_DECLARE_PREVIOUS(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            value_t* prefix##_previous(prefix##_t* map, value_t* value) { \
                mirror_iwbt_node_t* node; \
                ghost_assert(map != ghost_null, ""); \
                node = mirror_iwbt_previous(&map->tree, &value->node_field); \
                return (node == ghost_null) ? ghost_null : ghost_container_of(node, value_t, node_field); \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_PREVIOUS(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_PREVIOUS_MATCH(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that returns the previous element in the map.
 */
/* TODO get rid of all these docs, prefix all with IMPL */

/**
 * @def MIRROR_TMMAP_DEFINE_PREVIOUS_MATCH(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that returns the previous element in the map.
 */

/**
 * Returns the previous value with a matching key or null if this is the first
 * value with this key.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
value_t* mirror_tmmap_previous_match(mirror_tmmap_t* map, value_t* value);
#endif

        #define MIRROR_TMMAP_DECLARE_PREVIOUS_MATCH(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            value_t* prefix##_previous_match(prefix##_t* map, value_t* value) { \
                mirror_iwbt_node_t* previous_node; \
                value_t* previous_value; \
                ghost_assert(map != ghost_null, ""); \
                previous_node = mirror_iwbt_previous(&map->tree, &value->node_field); \
                if (previous_node == ghost_null) \
                    return ghost_null; \
                previous_value = ghost_container_of(previous_node, value_t, node_field); \
                if (0 != compare_fn(value_key_fn(value), value_key_fn(previous_value))) \
                    return ghost_null; \
                return previous_value; \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_PREVIOUS_MATCH(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_REMOVE_ALL_WITH_KEY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 */

/**
 * @def MIRROR_TMMAP_DEFINE_REMOVE_ALL_WITH_KEY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib)
 *
 */

/**
 * Removes all values with the given key from the map.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 *
 * @return The previous value or null
 */
void mirror_tmmap_remove_all_with_key(mirror_tmmap_t* map, key_t key);
#endif

        #include "ghost/impl/ghost_impl_function_wrapper.h"
        #define MIRROR_TMMAP_DECLARE_REMOVE_ALL_WITH_KEY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            noninline_attrib \
            void prefix##_remove_all_with_key(prefix##_t* map, key_t key); \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_REMOVE_ALL_WITH_KEY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            \
            noninline_attrib \
            void prefix##_remove_all_with_key(prefix##_t* map, key_t key) { \
                value_t* value; \
                for (value = prefix##_find_first(map, key); value != ghost_null;) { \
                    value_t* next_value = prefix##_next_match(map, value); \
                    prefix##_remove(map, value); \
                    value = next_value; \
                } \
            } \
            \
            GHOST_IMPL_FUNCTION_CLOSE

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_REMOVE_ANY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that removes a value by key from a map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_REMOVE_ANY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that removes a value by key from a map.
 */

/**
 * Removes any value from the given map and returns it, or returns null if the
 * map is empty.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
value_t* mirror_tmmap_remove_any(mirror_tmmap_t* map);
#endif

        #define MIRROR_TMMAP_DECLARE_REMOVE_ANY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            value_t* prefix##_remove_any(prefix##_t* map) { \
                mirror_iwbt_node_t* node; \
                ghost_assert(map != ghost_null, ""); \
                node = mirror_iwbt_remove_any(&map->tree); \
                return (node == ghost_null) ? ghost_null : ghost_container_of(node, value_t, node_field); \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_REMOVE_ANY(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_REMOVE_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that removes a value by key from a map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_REMOVE_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that removes a value by key from a map.
 */

/**
 * Removes and returns the first value in the map with the given key, or
 * returns null if the map contains no values with the given key.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
value_t* mirror_tmmap_remove_first(mirror_tmmap_t* map, key_t key);
#endif

        #define MIRROR_TMMAP_DECLARE_REMOVE_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            value_t* prefix##_remove_first(prefix##_t* map, key_t key) { \
                value_t* value; \
                ghost_assert(map != ghost_null, ""); \
                value = prefix##_find_first(map, key); \
                if (value != ghost_null) \
                    mirror_iwbt_remove(&map->tree, &value->node_field); \
                return value; \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_REMOVE_FIRST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_REMOVE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that removes a value from a map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_REMOVE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that removes a value from a map.
 */

/**
 * Removes the given value from the given map.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
void mirror_tmmap_remove(mirror_tmmap_t* map, value_t* value);
#endif

        #define MIRROR_TMMAP_DECLARE_REMOVE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            void prefix##_remove(prefix##_t* map, value_t* value) { \
                ghost_assert(map != ghost_null, ""); \
                ghost_assert(value != ghost_null, ""); \
                mirror_iwbt_remove(&map->tree, &value->node_field); \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_REMOVE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_REMOVE_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that removes a value by key from a map.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_REMOVE_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that removes a value by key from a map.
 */

/**
 * Removes and returns the last value in the map with the given key, or
 * returns null if the map contains no values with the given key.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
value_t* mirror_tmmap_remove_last(mirror_tmmap_t* map, key_t key);
#endif

        #define MIRROR_TMMAP_DECLARE_REMOVE_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            value_t* prefix##_remove_last(prefix##_t* map, key_t key) { \
                value_t* value; \
                ghost_assert(map != ghost_null, ""); \
                value = prefix##_find_last(map, key); \
                if (value != ghost_null) \
                    mirror_iwbt_remove(&map->tree, &value->node_field); \
                return value; \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_REMOVE_LAST(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE_SWAP(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Declares a function that swaps two maps.
 */

/**
 * @def MIRROR_TMMAP_DEFINE_SWAP(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, inline_attrib, noninline_attrib)
 *
 * Defines a function that swaps two maps.
 */

/**
 * Swaps the contents of two maps.
 *
 * This is a template function that must be instantiated. The prefix
 * "mirror_tmmap" is replaced by the prefix of the template.
 */
void mirror_tmmap_swap(mirror_tmmap_t* first, mirror_tmmap_t* second);
#endif

        #define MIRROR_TMMAP_DECLARE_SWAP(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib, inline_attrib) \
            GHOST_IMPL_FUNCTION_OPEN \
            inline_attrib \
            void prefix##_swap(prefix##_t* first, prefix##_t* second) { \
                ghost_assert(first != ghost_null, ""); \
                ghost_assert(second != ghost_null, ""); \
                mirror_iwbt_swap(&first->tree, &second->tree); \
            } \
            GHOST_IMPL_FUNCTION_CLOSE

        #define MIRROR_TMMAP_DEFINE_SWAP(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, noninline_attrib) \
            /*nothing*/

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DECLARE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn)
 *
 * Declares functions for an intrusive weight-balanced tree multimap.
 *
 * You must have first instantiated the type with
 * MIRROR_TMMAP_DECLARE_TYPE(), and you must define functions after this
 * declaration in exactly one translation unit using MIRROR_TMMAP_DEFINE().
 *
 * @see MIRROR_TMMAP_DECLARE_TYPE()
 * @see MIRROR_TMMAP_DEFINE()
 */
#endif

        #define MIRROR_TMMAP_DECLARE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn) \
            GHOST_IMPL_TMMAP_DECLARE_FUNCTIONS(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, /*nothing*/, static ghost_inline_opt)

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_DEFINE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn)
 *
 * Defines functions for an intrusive weight-balanced tree multimap.
 *
 * You must have first instantiated the type with
 * MIRROR_TMMAP_DEFINE_TYPE() and declared the functions with
 * MIRROR_TMMAP_DECLARE(). This must appear in exactly one translation unit
 * for a given map type.
 *
 * If you don't want to have to manually instantiate definitions, declare your
 * functions with MIRROR_TMMAP_HEADER() instead.
 *
 * @see MIRROR_TMMAP_DEFINE_TYPE()
 * @see MIRROR_TMMAP_DEFINE()
 */
#endif

        #define MIRROR_TMMAP_DEFINE(prefix, key_t, value_t, node_field, value_key_fn, compare_fn) \
            GHOST_IMPL_TMMAP_DEFINE_FUNCTIONS (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, /*nothing*/)

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP(prefix, key_t, value_t, node_field, value_key_fn, compare_fn)
 *
 * Declares and defines an intrusive weight-balanced tree multimap.
 *
 * This declares the map type and declares and defines all functions with
 * Ghost's header linkage, suitable for use in a header or source file.
 *
 * This defines functions in the same way Ghost does when not using
 * GHOST_MANUAL_DEFS. Small functions are declared `ghost_header_inline` which
 * generally means `static inline`. Large functions are declared
 * `ghost_header_function` which generally means selectany/comdat/weak, this
 * way only one definition will exist in the file link.
 *
 * This minimizes code bloat automatically with no manual effort required. If
 * you're unsure of what to use, use this.
 */
#endif

        #define MIRROR_TMMAP(prefix, key_t, value_t, node_field, value_key_fn, compare_fn) \
            MIRROR_TMMAP_TYPE(prefix) \
            MIRROR_TMMAP_HEADER(prefix, key_t, value_t, node_field, value_key_fn, compare_fn)

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_HEADER(prefix, key_t, value_t, node_field, value_key_fn, compare_fn)
 *
 * Declares and defines functions for an intrusive weight-balanced tree
 * multimap with Ghost's fully automatic "header" linkage, suitable for use in
 * a header or source file.
 *
 * This defines functions in the same way Ghost does (when not using
 * GHOST_MANUAL_DEFS). Small functions are declared `ghost_header_inline` which
 * generally means `static inline`. Large functions are declared
 * `ghost_header_function` which generally means selectany/weak, this way only
 * one definition will exist in the final link.
 *
 * You must have first instantiated the type with
 * MIRROR_TMMAP_DECLARE_TYPE().
 *
 * @see MIRROR_TMMAP_DECLARE_TYPE()
 * @see ghost_header_inline
 * @see ghost_header_function
 */
#endif

        #define MIRROR_TMMAP_HEADER(prefix, key_t, value_t, node_field, value_key_fn, compare_fn) \
            GHOST_IMPL_TMMAP_DECLARE_FUNCTIONS(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, ghost_header_function, ghost_header_inline) \
            GHOST_IMPL_TMMAP_DEFINE_FUNCTIONS (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, ghost_header_function)

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_STATIC(prefix, key_t, value_t, node_field, value_key_fn, compare_fn)
 *
 * Declares and defines functions for an intrusive weight-balanced tree
 * multimap with static linkage, intended for use in a single translation unit.
 *
 * All functions are declared either `static` or `static inline`. You generally
 * shouldn't put this in a header file. For a shared header file, use
 * MIRROR_TMMAP_HEADER() instead, or consider using the separate
 * MIRROR_TMMAP_DECLARE() and MIRROR_TMMAP_DEFINE().
 *
 * You must have first instantiated the type with
 * MIRROR_TMMAP_DECLARE_TYPE().
 *
 * @see MIRROR_TMMAP_DECLARE_TYPE()
 * @see MIRROR_TMMAP_HEADER()
 */
#endif

        #define MIRROR_TMMAP_STATIC(prefix, key_t, value_t, node_field, value_key_fn, compare_fn) \
            GHOST_IMPL_TMMAP_DECLARE_FUNCTIONS(prefix, key_t, value_t, node_field, value_key_fn, compare_fn, ghost_maybe_unused static, static ghost_inline_opt) \
            GHOST_IMPL_TMMAP_DEFINE_FUNCTIONS (prefix, key_t, value_t, node_field, value_key_fn, compare_fn, ghost_maybe_unused static)

#if MIRROR_IMPL_TMMAP_DOCUMENTATION
/**
 * @def MIRROR_TMMAP_TYPE(prefix)
 *
 * Declares an intrusive weight-balanced tree map type with the given prefix.
 */

/**
 * An intrusive weight-balanced tree map type with the given prefix.
 *
 * The map must be initialized by zeroing (via e.g. GHOST_ZERO_INIT) or by
 * calling mirror_tmmap_clear(). The map does not need to be de-initialized
 * (but you are responsible for freeing the contents if necessary.)
 *
 * @see mirror_tmmap_clear()
 */
typedef /*unspecified*/ mirror_tmmap_t;
#endif

        #define MIRROR_TMMAP_TYPE(prefix) \
            typedef struct prefix##_t { \
                mirror_iwbt_t tree; \
            } prefix##_t;

#endif
