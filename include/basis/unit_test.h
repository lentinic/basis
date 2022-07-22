/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

#include <stdio.h>

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
    BASIS_MACRO_BLOCK( \
        if (!(cnd)) \
        { \
            ErrorCount++; \
        } \
    )

#define BASIS_TEST_VERIFY_MSG(cnd, fmt, ...) \
    BASIS_MACRO_BLOCK( \
        if (!(cnd)) \
        { \
            ErrorCount++; \
            fprintf(stderr, "Error (%s:%d): "  fmt  "\n", __FILE__, __LINE__, __VA_ARGS__); \
        } \
    )

#define BASIS_RUN_TESTS() \
    auto start = basis::GetTimestamp(); \
    for (CurrentTest=0; CurrentTest<TestCount; CurrentTest++) \
    { \
        fprintf(stdout, "Running test \"%s\"\n", TestList[CurrentTest].TestName); \
        TestList[CurrentTest].TestFunction(); \
    } \
    fprintf(stdout, "%u tests completed in %.2f seconds.  Errors: %u\n", \
        TestCount, basis::GetTimeElapsedMS(start)/ 1000.f, ErrorCount);


