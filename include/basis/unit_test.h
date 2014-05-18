/*
Copyright (c) 2014 Chris Lentini
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

#include <stdio.h>

#include "debug.h"
#include "macros.h"
#include "timer.h"

#define BASIS_TEST_LIST_BEGIN() \
	unsigned ErrorCount = 0; \
	unsigned CurrentTest = 0; \
	struct TestEntry \
	{ \
		void (*TestFunction)(); \
		const char * TestName; \
	}; \
	TestEntry TestList[] = {

#define BASIS_TEST_LIST_END() \
		{ nullptr, nullptr } \
	}; \
	unsigned TestCount = (sizeof(TestList) / sizeof(TestEntry)) - 1;

#define BASIS_DECLARE_TEST(fn) \
	{ &fn, #fn },

#define BASIS_TEST_VERIFY(cnd) \
	BASIS_MACRO_BLOCK_BEGIN \
		if (!(cnd)) \
		{ \
			ErrorCount++; \
		} \
	BASIS_MACRO_BLOCK_END

#define BASIS_TEST_VERIFY_MSG(cnd, fmt, ...) \
	BASIS_MACRO_BLOCK_BEGIN \
		if (!(cnd)) \
		{ \
			ErrorCount++; \
			fprintf(stderr, "Error (%s:%d): "  fmt  "\n", __FILE__, __LINE__, __VA_ARGS__); \
		} \
	BASIS_MACRO_BLOCK_END

#define BASIS_RUN_TESTS() \
	auto start = basis::GetTimestamp(); \
	for (CurrentTest=0; CurrentTest<TestCount; CurrentTest++) \
	{ \
		fprintf(stdout, "Running test \"%s\"\n", TestList[CurrentTest].TestName); \
		TestList[CurrentTest].TestFunction(); \
	} \
	fprintf(stdout, "%u tests completed in %.2f seconds.  Errors: %u\n", \
		TestCount, basis::GetTimeElapsedMS(start)/ 1000.f, ErrorCount);


