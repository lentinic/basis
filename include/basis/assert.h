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

#define BASIS_ASSERT(cnd) BASIS_MACRO_BLOCK_BEGIN \
	if (!(cnd)) { basis::DebugBreak(); } \
BASIS_MACRO_BLOCK_END

#define BASIS_ASSERT_SIGNAL(cnd, signal) BASIS_MACRO_BLOCK_BEGIN \
	if (!(cnd) && (signal)) { basis::DebugBreak(); } \
BASIS_MACRO_BLOCK_END

#define BASIS_ASSERT_FAILED BASIS_MACRO_BLOCK_BEGIN basis::DebugBreak(); BASIS_MACRO_BLOCK_END

#else

#define BASIS_ASSERT(cnd) BASIS_UNUSED(cnd)
#define BASIS_ASSERT_SIGNAL(cnd, signal) BASIS_UNUSED(cnd); BASIS_UNUSED(signal)
#define BASIS_ASSERT_FAILED

#endif
