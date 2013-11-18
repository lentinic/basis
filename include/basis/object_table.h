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

#include <vector>
#include <stdint.h>

#include "assert.h"
#include "handle.h"

namespace basis
{
	template<class DATA_TYPE>
	class object_table
	{
	public:
		object_table()
			:	m_freelist(0)
		{}
		
		explicit object_table(size_t reserve)
			:	m_objects(count),
				m_external(count),
				m_internal(count),
				m_freelist(0)
		{}

		handle32 add(const DATA_TYPE & o)
		{
			handle32 h = alloc_handle();
			m_objects.push_back(o);
			m_external.push_back(h);

			return h;
		}

		handle32 add(DATA_TYPE && o)
		{
			handle32 h = alloc_handle();
			m_objects.push_back(o);
			m_external.push_back(h);

			return h;
		}

		void remove(handle32 h)
		{
			BASIS_ASSERT(exists(h));

			handle32 internal = m_internal[h.id];
			uint32_t last = m_objects.size() - 1;

			if (internal.id < last)
			{
				m_objects[internal.id] = std::move(m_objects[last]);
				m_external[internal.id] = m_external[last];
				m_internal[m_external[last].id].id = internal.id;
			}

			m_objects.pop_back();
			m_external.pop_back();

			internal.id = m_freelist;
			internal.generation++;
			m_internal[h.id] = internal;
			m_freelist = h.id;
		}

		bool exists(handle32 h) const
		{
			if (h.id >= m_internal.size())
				return false;
			
			if (h.generation != m_internal[h.id].generation)
				return false;

			BASIS_ASSERT(m_internal[h.id].id < m_external.size());
			BASIS_ASSERT(m_external[m_internal[h.id].id].id == h.id);
			BASIS_ASSERT(m_external[m_internal[h.id].id].generation == h.generation);

			return true;
		}

		DATA_TYPE & lookup(handle32 h) const
		{
			BASIS_ASSERT(exists(h));
			return m_objects[m_internal[h.id].id];
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
			return m_objects.size();
		}

		typedef typename std::vector<DATA_TYPE>::iterator iterator;
		typedef typename std::vector<DATA_TYPE>::const_iterator const_iterator;

		iterator begin() { return m_objects.begin(); }
		const_iterator begin() const { return m_objects.begin(); }
		const iterator cbegin() const { return m_objects.cbegin(); }
		iterator end() { return m_objects.end(); }
		const_iterator end() const { return m_objects.end(); }
		const iterator cend() const { return m_objects.cend(); }

	private:
		std::vector<DATA_TYPE> 	m_objects;
		std::vector<handle32>	m_external;
		std::vector<handle32>	m_internal;
		uint32_t				m_freelist;

		handle32 alloc_handle()
		{
			uint32_t index = m_freelist;

			BASIS_ASSERT(index <= m_internal.size());

			if (index == m_internal.size())
			{
				handle32 internal = { m_objects.size(), 0 };
				handle32 external = { index, 0 };
				m_internal.push_back(internal);
				m_freelist++;
				return external;
			}
			
			handle32 internal = m_internal[index];
			handle32 external = { index, internal.generation };
			internal.id = m_objects.size();
			m_internal[index] = internal;
			return external;
		}
	};
}
