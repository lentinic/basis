/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

#include <stdint.h>

namespace basis
{
    template<class T, class STORAGE>
    class handle
    {
    public:
        handle(STORAGE id) : m_data(id) {}

        operator bool () const { return m_data != 0; }
        operator STORAGE () const { return m_data; }
        
    private:
        STORAGE m_data {};

        template<class U> friend class object_table;
    };

    template<class T> using handle32_t = handle<T, uint32_t>;
    template<class T> using handle64_t = handle<T, uint64_t>;
}
 