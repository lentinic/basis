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
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "assert.h"

namespace basis
{
    class shared_mutex
    {
    public:
        bool try_lock_shared()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (!!(m_state & FLAG_BLOCK_SHARED))
                return false;
            m_state++;
            return true;
        }

        void lock_shared()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while (!!(m_state & FLAG_BLOCK_SHARED))
            {
                m_cond.wait(lock);
            }
            m_state++;
        }

        void unlock_shared()
        {    
            uint32_t prev;
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                prev = m_state--;
                BASIS_ASSERT(prev > 0);
            }

            if ((prev & MASK_COUNT_SHARED) == 1)
            {
                m_cond.notify_all();
            }
        }

        bool try_lock()
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            if (m_state == 0)
            {
                m_state |= FLAG_BLOCK_SHARED;
                return true;
            }

            return false;
        }

        void lock()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            uint32_t state = m_state;
            if (!(state & FLAG_BLOCK_SHARED))
            {
                m_state = state | FLAG_BLOCK_SHARED;
                while (state != 0)
                {
                    m_cond.wait(lock);
                    state = m_state & MASK_COUNT_SHARED;
                }
                return;
            }

            do
            {
                m_cond.wait(lock);
            } while (m_state != 0);
            m_state = FLAG_BLOCK_SHARED;
        }
        
        void unlock()
        {
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                BASIS_ASSERT(!!(m_state & FLAG_BLOCK_SHARED));
                m_state = 0;
            }
            m_cond.notify_all();
        }

    private:
        std::mutex                     m_mutex;
        std::condition_variable        m_cond;
        uint32_t                    m_state = 0;

        const uint32_t MASK_COUNT_SHARED = 0x7fffffff;
        const uint32_t FLAG_BLOCK_SHARED = 0x80000000;
    };
}
