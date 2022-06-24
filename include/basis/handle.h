/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

#include <stdint.h>

namespace basis
{
    template<class T, class STORAGE = uint32_t, unsigned ID_BITS = 24, unsigned GEN_BITS = 8>
    struct handle
    {
        static_assert((ID_BITS + GEN_BITS) == (sizeof(STORAGE) * 8), 
            "handle bit total must match the specified storage type");

        STORAGE    id : ID_BITS;
        STORAGE    generation : GEN_BITS;

        static const uint32_t max_id = ((1 << ID_BITS) - 1);
    };
}
 