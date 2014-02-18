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

namespace basis
{
	class bitset
	{
	public:
		explicit bitset(size_t count)
			:	m_bitCount(count),
				m_intCount(count / INT_BITS)
		{
			m_intCount = (m_intCount == 0) ? 1 : m_intCount;
			m_bits = new int[m_intCount];
			memset(m_bits, 0, sizeof(int) * m_intCount);
		}

		bitset(const bitset & other)
		{
			m_bitCount = other.m_bitCount;
			m_intCount = other.m_intCount;
			m_bits = new int[m_intCount];
			memcpy(m_bits, other.m_bits, sizeof(int) * m_intCount);
		}

		bitset & operator = (const bitset & other)
		{
			m_bitCount = other.m_bitCount;
			m_intCount = other.m_intCount;
			delete [] m_bits;
			m_bits = new int[m_intCount];
			memcpy(m_bits, other.m_bits, sizeof(int) * m_intCount);
			return *this;
		}

		bitset(bitset && other)
		{
			m_bitCount = other.m_bitCount;
			m_intCount = other.m_intCount;
			m_bits = other.m_bits;
			other.m_bits = nullptr;

		}

		bitset & operator = (bitset && other)
		{
			m_bitCount = other.m_bitCount;
			m_intCount = other.m_intCount;
			delete [] m_bits;
			m_bits = other.m_bits;
			other.m_bits = nullptr;			
			return *this;	
		}

		int set_or(size_t index, int b)
		{
			size_t pos;
			get_offsets(index, &pos, &index);

			int r = m_bits[pos] |= (b << index);
			return (r >> index) & 1;
		}

		int set_xor(size_t index, int b)
		{
			size_t pos;
			get_offsets(index, &pos, &index);

			int r = m_bits[pos] ^= (b << index);
			return (r >> index) & 1;
		}

		int set_and(size_t index, int b)
		{
			size_t pos;
			get_offsets(index, &pos, &index);

			int r = m_bits[pos] &= (b << index);
			return (r >> index) & 1;
		}

		void set_all_one()
		{
			memset(m_bits, 0xff, m_intCount * sizeof(int));
		}

		void set_all_zero()
		{
			memset(m_bits, 0xff, m_intCount * sizeof(int));
		}

		bool test_all_one()
		{
			int * cur = m_bits;
			int * end = cur + m_intCount;
			int v = 0;
			while (cur != end)
			{
				v = *cur;
				cur++;

				if (v != 0xffffffff)
					break;
			}

			if (cur == end)
			{
				size_t leftover = (m_intCount * INT_BITS) - m_bitCount;
				int mask = 0xffffffff >> leftover;
				return (v & mask) == mask;
			}

			return false;
		}

	private:
		size_t		m_bitCount;
		size_t		m_intCount;
		int * 		m_bits;

		void get_offsets(size_t index, size_t * a, size_t * b)
		{
			size_t pos = index / INT_BITS;
			BASIS_ASSERT(pos < m_intCount);
			*a = pos;
			*b = index - (pos * INT_BITS);
		}

		static const size_t INT_BITS = (sizeof(int) * 8); 
	};
}
 