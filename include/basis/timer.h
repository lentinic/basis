/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__linux__)
#include <time.h>
#else
#include <chrono>
#endif

namespace basis
{
    typedef uint64_t tick_t;

    inline tick_t GetTimestamp()
    {
        tick_t now;
#if defined(_WIN32) || defined(_WIN64)
        QueryPerformanceCounter((LARGE_INTEGER *)&now);
#elif defined(__linux__)
        timespec t;
        clock_gettime(CLOCK_MONOTONIC, &t);
        now = t.tv_sec * 1000000000 +
              t.tv_nsec;
#else
        auto t = std::chrono::steady_clock::now();
        now = std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count();
#endif
        return now;
    }

    inline tick_t GetTimerFrequency()
    {
#if defined(_WIN32) || defined(_WIN64)
        tick_t freq;
        QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
        return freq;
#else
        return 1000000000;
#endif    
    }

    inline tick_t GetTimeDeltaMS(tick_t start, tick_t end)
    {
        tick_t freq = GetTimerFrequency();
        return ((end - start) * 1000) / freq;
    }

    inline tick_t GetTimeDeltaUS(tick_t start, tick_t end)
    {
        tick_t freq = GetTimerFrequency();
        return ((end - start) * 1000000) / freq;
    }

    inline tick_t GetTimeDeltaNS(tick_t start, tick_t end)
    {
        tick_t freq = GetTimerFrequency();
        return ((end - start) * 1000000000) / freq;    
    }

    inline tick_t GetTimeElapsedMS(tick_t start)
    {
        tick_t now = GetTimestamp();
        return GetTimeDeltaMS(start, now);
    }

    inline tick_t GetTimeElapsedUS(tick_t start)
    {
        tick_t now = GetTimestamp();
        return GetTimeDeltaUS(start, now);
    }

    inline tick_t GetTimeElapsedNS(tick_t start)
    {
        tick_t now = GetTimestamp();
        return GetTimeDeltaNS(start, now);
    }
}
 