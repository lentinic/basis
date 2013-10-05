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

namespace basis
{
	template<class DATA_TYPE>
	class object_table
	{
	public:
		object_table() :
			m_freelist(0)
		{}	

		uint32_t add(const DATA_TYPE & o)
		{
			uint32_t id = reserve_id();

			m_ids.push_back(id);
			m_objects.push_back(o);

			return id;
		}

		uint32_t add(DATA_TYPE && o)
		{
			uint32_t id = reserve_id();

			m_ids.push_back(id);
			m_objects.push_back(o);

			return id;	
		}

		void remove(uint32_t id)
		{
			BASIS_ASSERT(exists(id));

			uint32_t index = m_lookup[id];
			uint32_t last = m_objects.size() - 1;

			if (index < last)
			{
				m_objects[index] = std::move(m_objects[last]);
				m_ids[index] = m_ids[last];
				m_lookup[m_ids[index]] = index;
			}

			m_objects.pop_back();
			m_ids.pop_back();
			m_lookup[id] = m_freelist;
			m_freelist = id;
		}

		bool exists(uint32_t id) const
		{
			if (id >= m_lookup.size())
				return false;
			uint32_t index = m_lookup[id];
			if (index >= m_ids.size())
				return false;
			return id == m_ids[index];
		}

		DATA_TYPE & lookup(uint32_t id) const
		{
			BASIS_ASSERT(exists(id));
			return m_objects[m_lookup[id]];
		}

		void clear()
		{
			m_objects.clear();
			m_ids.clear();
			m_lookup.clear();
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
		std::vector<uint32_t>	m_ids;
		std::vector<uint32_t>	m_lookup;
		uint32_t				m_freelist;

		uint32_t reserve_id()
		{
			uint32_t id = m_freelist;
	
			BASIS_ASSERT(id <= m_lookup.size());

			if (id == m_lookup.size())
			{
				m_lookup.push_back(m_objects.size());
				m_freelist++;
			}
			else
			{
				m_freelist = m_lookup[id];
				m_lookup[id] = m_objects.size();
			}

			return id;
		}
	};
}
