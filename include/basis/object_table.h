/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

#include <vector>
#include <utility>

#include <stdint.h>

#include "assert.h"
#include "handle.h"

namespace basis
{
    template<class data_t>
    class object_table
    {
    public:
        using handle_t = handle32_t<data_t>;

    public:
        object_table()
        :    m_freelist(0)
        {}
        
        explicit object_table(size_t reserve)
        :   m_objects(count),
            m_external(count),
            m_internal(count),
            m_freelist(0)
        {}

        handle_t add(const data_t & o)
        {
            handle_t h = alloc_handle();
            m_objects.push_back(o);
            m_external.push_back(h);

            return h;
        }

        handle_t add(data_t && o)
        {
            handle_t h = alloc_handle();
            m_objects.push_back(o);
            m_external.push_back(h);

            return h;
        }

        void remove(handle_t h)
        {
            BASIS_ASSERT(exists(h));

            handle_view vh(h);
            handle_view vi(m_internal[vh.id]);
            
            uint32_t last = (uint32_t)(m_objects.size() - 1);

            if (vi.id < last)
            {
                m_objects[vi.id] = std::move(m_objects[last]);
                m_external[vi.id] = m_external[last];

                handle_view vel(m_external[last]);
                handle_view vil(m_internal[vel.id]);
                vil.id = vi.id;
                m_internal[vel.id] = vil.handle;
            }

            m_objects.pop_back();
            m_external.pop_back();

            vi.id = m_freelist;
            vi.generation++;
            m_internal[vh.id] = vi.handle;
            m_freelist = vh.id;
        }

        bool exists(handle_t h) const
        {
            handle_view view(h);

            if (view.id >= m_internal.size())
                return false;
            
            if (view.generation != handle_view(m_internal[view.id]).generation)
                return false;

            BASIS_ASSERT(handle_view(m_internal[view.id]).id < m_external.size());
            BASIS_ASSERT(handle_view(m_external[handle_view(m_internal[view.id]).id]).id == view.id);
            BASIS_ASSERT(handle_view(m_external[handle_view(m_internal[view.id]).id]).generation == view.generation);

            return true;
        }

        data_t & lookup(handle_t h) const
        {
            BASIS_ASSERT(exists(h));
            
            handle_view internal(m_internal[handle_view(h).id]);
            return m_objects[internal.id];
        }

        void clear()
        {
            m_objects.clear();
            m_external.clear();
            m_internal.clear();
            m_freelist = 0;			
        }

        uint32_t count() const
        {
            return (uint32_t) m_objects.size();
        }

        typedef typename std::vector<data_t>::iterator iterator;
        typedef typename std::vector<data_t>::const_iterator const_iterator;

        iterator begin() { return m_objects.begin(); }
        const_iterator begin() const { return m_objects.begin(); }
        const iterator cbegin() const { return m_objects.cbegin(); }
        iterator end() { return m_objects.end(); }
        const_iterator end() const { return m_objects.end(); }
        const iterator cend() const { return m_objects.cend(); }

    private:
        std::vector<data_t>        m_objects;
        std::vector<handle_t>      m_external;
        std::vector<handle_t>      m_internal;
        uint32_t                   m_freelist;

        struct handle_view
        {
            handle_view(handle_t h) 
            :   handle(h) 
            {}
            
            handle_view(uint32_t id, uint32_t gen)
            :   id(id),
                generation(gen)
            {}

            union
            {
                handle_t handle;
                struct
                {
                    uint32_t id         : 24;
                    uint32_t generation : 8;
                };
            };

            static constexpr uint32_t max_id = (1<<24) - 1;
        };

        handle_t alloc_handle()
        {
            uint32_t index = m_freelist;

            BASIS_ASSERT(index <= m_internal.size());

            if (index == m_internal.size())
            {
                BASIS_ASSERT(index <= handle_view::max_id);

                handle_t internal = handle_view((uint32_t) m_objects.size(), 0).handle;
                handle_t external = handle_view(index, 0).handle;
                
                m_internal.push_back(internal);
                m_freelist++;

                return external;
            }
            
            handle_view internal(m_internal[index]);
            internal.id = (uint32_t) m_objects.size();
            m_internal[index] = internal.handle;

            handle_t external = handle_view(index, internal.generation).handle;
            return external;
        }
    };
}
