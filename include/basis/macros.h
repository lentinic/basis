/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

#define BASIS_MACRO_BLOCK(macro) do { \
        macro \
    } while((void)0, 0)

#define BASIS_UNUSED_1(_1) BASIS_MACRO_BLOCK( \
        (void)(true ? (void)0 : ((void)(_1))); \
    )
#define BASIS_UNUSED_2(_1, _2) BASIS_UNUSED_1(_1); BASIS_UNUSED_1(_2)
#define BASIS_UNUSED_3(_1, _2, _3) BASIS_UNUSED_1(_1); BASIS_UNUSED_2(_2, _3)
#define BASIS_UNUSED_4(_1, _2, _3, _4) BASIS_UNUSED_1(_1); BASIS_UNUSED_3(_2, _3, _4)
#define BASIS_UNUSED_5(_1, _2, _3, _4, _5) BASIS_UNUSED_1(_1); BASIS_UNUSED_4(_2, _3, _4, _5)
#define BASIS_UNUSED_6(_1, _2, _3, _4, _5, _6) BASIS_UNUSED_1(_1); BASIS_UNUSED_5(_2, _3, _4, _5, _6)
#define BASIS_UNUSED_7(_1, _2, _3, _4, _5, _6, _7) BASIS_UNUSED_1(_1); BASIS_UNUSED_6(_2, _3, _4, _5, _6, _7)
#define BASIS_UNUSED_8(_1, _2, _3, _4, _5, _6, _7, _8) BASIS_UNUSED_1(_1); BASIS_UNUSED_7(_2, _3, _4, _5, _6, _7, _8)

#define BASIS_UNUSED_SELECTOR(tuple) BASIS_UNUSED_SELECTOR_IMPL tuple
#define BASIS_UNUSED_SELECTOR_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,N,...) N
#define BASIS_UNUSED(...) BASIS_UNUSED_SELECTOR((__VA_ARGS__,BASIS_UNUSED_8,BASIS_UNUSED_7,BASIS_UNUSED_6, \
    BASIS_UNUSED_5, BASIS_UNUSED_4, BASIS_UNUSED_3, BASIS_UNUSED_2, BASIS_UNUSED_1))(__VA_ARGS__)

#define BASIS_CONCAT_BASE(x,y) x ## y
#define BASIS_CONCAT(x,y) BASIS_CONCAT_BASE(x,y)

#if defined(_MSC_VER)
#define BASIS_NOINLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
#define BASIS_NOINLINE __attribute__((noinline))
#else
#define BASIS_NOINLINE
#endif

