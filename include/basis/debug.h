/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

namespace basis
{
    inline void DebugBreak()
    {
#if defined(_MSC_VER)
        __debugbreak();
#elif defined(__GNUC__) || defined(__clang__)
        __builtin_trap();
#elif defined(__x86_64__) || defined(__i386__)
        __asm__ ("int $3");
#else
        *((int *) 0) = 3;
#endif
    }
}
