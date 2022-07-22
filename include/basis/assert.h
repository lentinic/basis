/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

#include "macros.h"

#if defined(_MSC_VER)
#define BASIS_BREAKPOINT() BASIS_MACRO_BLOCK(__debugbreak();)
#elif defined(__GNUC__) || defined(__clang__)
#define BASIS_BREAKPOINT() BASIS_MACRO_BLOCK(__builtin_trap();)
#elif defined(__x86_64__) || defined(__i386__)
#define BASIS_BREAKPOINT() BASIS_MACRO_BLOCK(__asm__ ("int $3");)
#else
#define BASIS_BREAKPOINT() BASIS_MACRO_BLOCK(*((int *) 0) = 3; })
#endif

#if defined(_DEBUG) || defined(BASIS_ASSERT_ENABLED)

#define BASIS_ASSERT_BASIC(cnd) \
    BASIS_MACRO_BLOCK( if (!(cnd)) { BASIS_BREAKPOINT(); } )

#define BASIS_ASSERT_SIGNAL(cnd, signal) \
    BASIS_MACRO_BLOCK( if (!(cnd) && (signal)()) { BASIS_BREAKPOINT(); } )

#define BASIS_ASSERT_FAILED BASIS_BREAKPOINT()

#else

#define BASIS_ASSERT_BASIC(cnd) BASIS_UNUSED(cnd)
#define BASIS_ASSERT_SIGNAL(cnd, signal) BASIS_UNUSED(cnd); BASIS_UNUSED(signal)
#define BASIS_ASSERT_FAILED
 
#endif
 
#define BASIS_ASSERT_SELECTOR(tuple) BASIS_ASSERT_SELECTOR_IMPL tuple
#define BASIS_ASSERT_SELECTOR_IMPL(_1,_2,N,...) N

#define BASIS_ASSERT(...) BASIS_ASSERT_SELECTOR((__VA_ARGS__,BASIS_ASSERT_SIGNAL,BASIS_ASSERT_BASIC))(__VA_ARGS__)
