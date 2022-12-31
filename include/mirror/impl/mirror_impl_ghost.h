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

#ifndef MIRROR_IMPL_GHOST_H
#define MIRROR_IMPL_GHOST_H

/* TODO prepare this to amalgamate ghost (maybe?) */

#include "ghost/debug/ghost_ensure.h"
#include "ghost/debug/ghost_fatal.h"
#include "ghost/detect/ghost_gcc.h"
#include "ghost/language/ghost_array_count.h"
#include "ghost/language/ghost_dllexport.h"
#include "ghost/language/ghost_zero_init.h"
#include "ghost/language/ghost_maybe_unused.h"
#include "ghost/language/ghost_discard.h"
#include "ghost/language/ghost_unreachable.h"
#include "ghost/language/ghost_static_cast.h"
#include "ghost/language/ghost_inline_opt.h"
#include "ghost/language/ghost_reinterpret_cast.h"
#include "ghost/language/ghost_static_init.h"
#include "ghost/format/ghost_snprintf.h"
#include "ghost/preprocessor/ghost_concat.h"
#include "ghost/preprocessor/ghost_count_args.h"
#include "ghost/preprocessor/ghost_counter.h"
#include "ghost/preprocessor/ghost_defer.h"
#include "ghost/preprocessor/ghost_expand.h"
#include "ghost/preprocessor/ghost_first.h"
#include "ghost/preprocessor/ghost_if_else.h"
#include "ghost/preprocessor/ghost_if_else_block.h"
#include "ghost/preprocessor/ghost_insert_comma.h"
#include "ghost/preprocessor/ghost_is_blank.h" /* TODO may not be needed */
#include "ghost/preprocessor/ghost_is_parens.h"
#include "ghost/preprocessor/ghost_msvc_traditional.h"
#include "ghost/preprocessor/ghost_rest.h"
#include "ghost/preprocessor/ghost_stringify.h"
#include "ghost/preprocessor/ghost_pp_va_args.h"

#include "ghost/type/char/ghost_char_is_signed.h"
#include "ghost/type/bool/ghost_bool.h"
#include "ghost/type/size_t/ghost_size_t.h"
#include "ghost/type/uint16_t/ghost_uint16_t.h"
#include "ghost/type/uint32_t/ghost_uint32_t.h"
#include "ghost/type/uint64_t/ghost_uint64_t.h"
#include "ghost/type/uint8_t/ghost_uint8_t.h"
#include "ghost/type/int16_t/ghost_int16_t.h"
#include "ghost/type/int32_t/ghost_int32_t.h"
#include "ghost/type/int64_t/ghost_int64_t.h"
#include "ghost/type/int8_t/ghost_int8_t.h"

#include "ghost/format/ghost_format_i16.h"
#include "ghost/format/ghost_format_i32.h"
#include "ghost/format/ghost_format_i64.h"
#include "ghost/format/ghost_format_i8.h"
#include "ghost/format/ghost_format_ip.h"
#include "ghost/format/ghost_format_pd.h"
#include "ghost/format/ghost_format_z.h"
#include "ghost/silence/ghost_silence_unused_parameter.h" /* TODO remove after removing mirror test parameter */
#include "ghost/silence/ghost_silence_align_padding.h" /* TODO remove after fixing load/store tests */
#include "ghost/silence/ghost_silence_insufficient_macro_args.h"
#include "ghost/string/ghost_bzero.h"
#include "ghost/string/ghost_strcmp.h"
#include "ghost/string/ghost_strcpy.h"

/* additional stuff needed for internal */
#include "ghost/container/template/tmmap/ghost_tmmap_type.h"
#include "ghost/container/template/tmmap/ghost_tmmap_static.h"
#include "ghost/debug/ghost_abort.h"
#include "ghost/language/ghost_noinline.h"
#include "ghost/language/ghost_extern_c_push_pop.h"
#include "ghost/malloc/ghost_calloc.h"
#include "ghost/malloc/ghost_alloc.h"
#include "ghost/malloc/ghost_free.h"
#include "ghost/malloc/ghost_alloca.h"
#include "ghost/malloc/ghost_alloc_zero.h"
#include "ghost/math/ghost_compare.h"
#include "ghost/language/ghost_null.h"
/*#include "ghost/malloc/ghost_alloca.h"//TODO */

#include "ghost/type/float32_t/ghost_float32_t.h"
#include "ghost/type/float64_t/ghost_float64_t.h"

#include "ghost/math/max/ghost_max_f.h"
#include "ghost/math/max/ghost_max_d.h"
#include "ghost/math/abs/ghost_abs_f.h"
#include "ghost/math/abs/ghost_abs_d.h"

#include "ghost/detect/ghost_gcc.h"

/* TODO avoid ghost impl */
#include "ghost/impl/format/ghost_impl_format_c99_specifiers.h"

/* TODO move these */
#define MIRROR_EAT_1(a)
#define MIRROR_EAT_2(a,b)
#define MIRROR_EAT_3(a,b,c)

#endif
