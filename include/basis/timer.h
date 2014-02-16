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
#endif

namespace basis
{
	typedef uint64_t timestamp;
	typedef uint64_t timedelta;

	timestamp GetTimestamp()
	{
#if defined(WIN32)
		timestamp now;
		QueryPerformanceCounter((LARGE_INTEGER *)&now);
		return now;
#else
		#error "basis::GetTimestamp not implemented for target platform"
#endif
	}

	timedelta GetTimeElapsedMS(timestamp start)
	{
#if defined(WIN32)
		timestamp now;
		uint64_t freq;
		QueryPerformanceCounter((LARGE_INTEGER *)&now);
		QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
		return ((now - start) * 1000) / freq;
#else
		#error "basis::GetTimeElapsedMS not implemented for target platform"
#endif
	}
}
 