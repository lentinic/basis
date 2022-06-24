/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

namespace basis
{
    template<unsigned ALIGN, typename ITYPE>
    inline ITYPE AlignValue(const ITYPE & val)
    {
        return (((val) + (ITYPE)(ALIGN-1)) & (~(ITYPE)(ALIGN - 1)));
    }
}
