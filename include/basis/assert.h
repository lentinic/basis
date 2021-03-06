/*
Copyright (c) 2013 Chris Lentini
http://divergentcoder.com

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to use, 
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "debug.h"
#include "macros.h"

#if defined(_DEBUG) || defined(BASIS_ASSERT_ENABLED)

#define BASIS_ASSERT_BASIC(cnd) \
	BASIS_MACRO_BLOCK( if (!(cnd)) { basis::DebugBreak(); } )

#define BASIS_ASSERT_SIGNAL(cnd, signal) \
	BASIS_MACRO_BLOCK( if (!(cnd) && (signal)()) { basis::DebugBreak(); } )

#define BASIS_ASSERT_FAILED BASIS_MACRO_BLOCK(basis::DebugBreak();)

#else

#define BASIS_ASSERT_BASIC(cnd) BASIS_UNUSED(cnd)
#define BASIS_ASSERT_SIGNAL(cnd, signal) BASIS_UNUSED(cnd); BASIS_UNUSED(signal)
#define BASIS_ASSERT_FAILED

#endif

#define BASIS_ASSERT_SELECTOR(tuple) BASIS_ASSERT_SELECTOR_IMPL tuple
#define BASIS_ASSERT_SELECTOR_IMPL(_1,_2,N,...) N

#define BASIS_ASSERT(...) BASIS_ASSERT_SELECTOR((__VA_ARGS__,BASIS_ASSERT_SIGNAL,BASIS_ASSERT_BASIC))(__VA_ARGS__)
