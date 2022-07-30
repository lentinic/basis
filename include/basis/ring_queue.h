/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

#include <atomic>
#include <utility>
#include "queue_access_policy.h"
#include "assert.h"

namespace basis
{
    namespace internal
    {
        const uint32_t slot_empty = 0x80000000;
        const uint32_t slot_mask = 0x7fffffff;

        template<class TYPE>
        struct ring_slot
        {
            TYPE                    m_data;
            std::atomic<uint32_t>   m_state;
        };
    }

    template<class TYPE, queue_access_policy ACCESS>
    class ring_queue;

    template<class TYPE>
    class ring_queue<TYPE, queue_access_policy::spsc>
    {
    public:
        explicit ring_queue(uint32_t size)
            :    m_items(new internal::ring_slot<TYPE>[size]),
                m_size(size),
                m_head(0),
                m_tail(0)
        {
            for (uint32_t i = 0; i < size; i++)
            {
                m_items[i].m_state.store(internal::slot_empty);
            }
        }

        ~ring_queue()
        {
            delete [] m_items;
        }

        template<class U>
        bool push_back(U && data)
        {
            auto tail = m_tail;
            auto s = &m_items[tail % m_size];
            if (s->m_state.load(std::memory_order_acquire) != internal::slot_empty)
            {
                return false;
            }

            s->m_data = std::forward<U>(data);
            s->m_state.store(tail, std::memory_order_release);
            m_tail = (tail + 1) & internal::slot_mask;

            return true;
        }

        bool pop_front(TYPE & dest)
        {
            auto head = m_head;
            auto s = &m_items[head % m_size];
            if (s->m_state.load(std::memory_order_acquire) == internal::slot_empty)
            {
                return false;
            }

            dest = std::move(s->m_data);
            s->m_state.store(internal::slot_empty, std::memory_order_release);
            m_head = (head + 1) & internal::slot_mask;

            return true;
        }

    private:
        ring_queue(const ring_queue &) = delete;
        ring_queue & operator = (const ring_queue &) = delete;

        internal::ring_slot<TYPE> *    m_items;
        const uint32_t                 m_size;
        uint32_t                    m_head;
        uint32_t                    m_tail;
    };

    template<class TYPE>
    class ring_queue<TYPE, queue_access_policy::spmc>
    {
    public:
        explicit ring_queue(uint32_t size)
            :    m_items(new internal::ring_slot<TYPE>[size]),
                m_size(size),
                m_head(0),
                m_tail(0)
        {
            for (uint32_t i = 0; i < size; i++)
            {
                m_items[i].m_state.store(internal::slot_empty);
            }
        }

        ~ring_queue()
        {
            delete [] m_items;
        }

        template<class U>
        bool push_back(U && data)
        {
            auto tail = m_tail;
            auto s = &m_items[tail % m_size];
            if (s->m_state.load(std::memory_order_acquire) != internal::slot_empty)
            {
                return false;
            }

            s->m_data = std::forward<U>(data);
            s->m_state.store(tail, std::memory_order_release);
            m_tail = (tail + 1) & internal::slot_mask;

            return true;
        }

        bool pop_front(TYPE & dest)
        {
            auto head = m_head.load(std::memory_order_acquire);
            auto s = &m_items[head % m_size];

            for (;;)
            {
                auto state = s->m_state.load(std::memory_order_acquire);
                if (state == head)
                {
                    if (m_head.compare_exchange_weak(head, (head + 1) & internal::slot_mask, std::memory_order_acq_rel, std::memory_order_acquire))
                    {
                        break;
                    }
                }
                else
                {
                    // the queue might not be empty - someone might have just beat us to the spot we just checked
                    auto current = m_head.load(std::memory_order_relaxed);
                    if (head == current)
                    {
                        return false;
                    }
                    head = current;
                }

                cpu_yield();
                s = &m_items[head % m_size];
            }

            dest = std::move(s->m_data);
            s->m_state.store(internal::slot_empty, std::memory_order_release);
            return true;
        }

    private:
        ring_queue(const ring_queue &) = delete;
        ring_queue & operator = (const ring_queue &) = delete;

        internal::ring_slot<TYPE> * m_items;
        const uint32_t                 m_size;
        std::atomic<uint32_t>        m_head;
        uint32_t                    m_tail;
    };

    template<class TYPE>
    class ring_queue<TYPE, queue_access_policy::mpsc>
    {
    public:
        explicit ring_queue(uint32_t size)
            :    m_items(new internal::ring_slot<TYPE>[size]),
                m_size(size),
                m_head(0),
                m_tail(0)
        {
            for (uint32_t i = 0; i < size; i++)
            {
                m_items[i].m_state.store(i | internal::slot_empty);
            }
        }

        ~ring_queue()
        {
            delete [] m_items;
        }

        template<class U>
        bool push_back(U && data)
        {
            auto tail = m_tail.load(std::memory_order_acquire);
            auto s = &m_items[tail % m_size];

            for (;;)
            {
                auto state = s->m_state.load(std::memory_order_acquire);
                if (state == (tail | internal::slot_empty))
                {
                    if (m_tail.compare_exchange_weak(tail, (tail + 1) & internal::slot_mask, std::memory_order_acq_rel, std::memory_order_acquire))
                    {
                        break;
                    }
                }
                else
                {
                    return false;
                }


                cpu_yield();
                s = &m_items[tail % m_size];
            }

            s->m_data = std::forward<U>(data);
            s->m_state.store(tail, std::memory_order_release);
            return true;
        }

        bool pop_front(TYPE & dest)
        {
            auto head = m_head;
            auto s = &m_items[head % m_size];
            if (!!(s->m_state.load(std::memory_order_acquire) & internal::slot_empty))
            {
                return false;
            }

            dest = std::move(s->m_data);
            auto next = (head + m_size) & internal::slot_mask;
            s->m_state.store(next | internal::slot_empty, std::memory_order_release);
            m_head = (head + 1) & internal::slot_mask;

            return true;
        }

    private:
        ring_queue(const ring_queue &) = delete;
        ring_queue & operator = (const ring_queue &) = delete;

        internal::ring_slot<TYPE> * m_items;
        const uint32_t                 m_size;
        uint32_t                    m_head;
        std::atomic<uint32_t>        m_tail;
    };

    template<class TYPE>
    class ring_queue<TYPE, queue_access_policy::mpmc>
    {
    public:
        explicit ring_queue(uint32_t size)
        :   m_items(new internal::ring_slot<TYPE>[size]),
            m_size(size),
            m_head(0),
            m_tail(0)
        {
            for (uint32_t i = 0; i < size; i++)
            {
                m_items[i].m_state.store(i | internal::slot_empty);
            }
        }

        ~ring_queue()
        {
            delete [] m_items;
        }

        template<class U>
        bool push_back(U && data)
        {
            auto tail = m_tail.load(std::memory_order_acquire);
            auto s = &m_items[tail % m_size];

            for (;;)
            {
                auto state = s->m_state.load(std::memory_order_acquire);
                if (state == (tail | internal::slot_empty))
                {
                    if (m_tail.compare_exchange_weak(tail, (tail + 1) & internal::slot_mask, std::memory_order_acq_rel, std::memory_order_acquire))
                    {
                        break;
                    }
                }
                else
                {
                    return false;
                }


                cpu_yield();
                s = &m_items[tail % m_size];
            }

            s->m_data = std::forward<U>(data);
            s->m_state.store(tail, std::memory_order_release);
            return true;
        }

        bool pop_front(TYPE & dest)
        {
            auto head = m_head.load(std::memory_order_acquire);
            auto s = &m_items[head % m_size];

            for (;;)
            {
                auto state = s->m_state.load(std::memory_order_acquire);
                if (state == head)
                {
                    if (m_head.compare_exchange_weak(head, (head + 1) & internal::slot_mask, std::memory_order_acq_rel, std::memory_order_acquire))
                    {
                        break;
                    }
                }
                else
                {
                    // the queue might not be empty - someone might have just beat us to the spot we just checked
                    auto current = m_head.load(std::memory_order_relaxed);
                    if (head == current)
                    {
                        return false;
                    }
                    head = current;
                }

                cpu_yield();
                s = &m_items[head % m_size];
            }

            dest = std::move(s->m_data);
            auto next = (head + m_size) & internal::slot_mask;
            s->m_state.store(next | internal::slot_empty, std::memory_order_release);
            return true;
        }

    private:
        ring_queue(const ring_queue &) = delete;
        ring_queue & operator = (const ring_queue &) = delete;

        internal::ring_slot<TYPE> * m_items;
        const uint32_t              m_size;
        std::atomic<uint32_t>       m_head;
        std::atomic<uint32_t>       m_tail;
    };
}
