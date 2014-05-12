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

#include <stdint.h>

#if defined(WIN32)
#include <windows.h>
#elif defined(LINUX)
#include <time.h>
#endif

namespace basis
{
	typedef uint64_t timestamp;
	typedef uint64_t timedelta;

	timestamp GetTimestamp()
	{
		timestamp now;
#if defined(WIN32)
		QueryPerformanceCounter((LARGE_INTEGER *)&now);
#elif defined(LINUX)
		timespec t;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
		now = t.tv_sec * 1000000000 + t.tv_nsec;
#else
		#error "basis::GetTimestamp not implemented for target platform"
#endif
		return now;
	}

	timedelta GetTimeElapsedMS(timestamp start)
	{
		timestamp now = GetTimestamp();
#if defined(WIN32)
		uint64_t freq;
		QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
		return ((now - start) * 1000) / freq;
#elif defined(LINUX)
		return (now - start) / (1000000); 
#else
		#error "basis::GetTimeElapsedMS not implemented for target platform"
#endif
	}
}
 