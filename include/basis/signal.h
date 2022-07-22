/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

#include <functional>
#include <stdint.h>

#include "object_table.h"

namespace basis
{
    template<class SIGNATURE> class signal;

    template<class... PARAMETERS>
    class signal<void(PARAMETERS...)>
    {
    public:
        using function_t = std::function<void(PARAMETERS...)>;
        using handle_t = handle32_t<function_t>;

        handle_t connect(function_t fn)
        {
            return m_listeners.add(fn);
        }

        void disconnect(handle_t id)
        {
            m_listeners.remove(id);
        }

        void reset()
        {
            m_listeners.clear();
        }

        void operator () (PARAMETERS... args)
        {
            for (auto fn : m_listeners)
            {
                fn(args...);
            }
        }

    private:
        object_table<function_t> m_listeners;
    };
}
