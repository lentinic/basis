/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

namespace basis
{
    inline void cpu_yield()
    {
#if defined(X86)
        _mm_pause();
#endif
    }
}

#if defined(_MSC_VER)
#define basis_thread_local __declspec(thread)
#else
#define basis_thread_local thread_local
#endif
