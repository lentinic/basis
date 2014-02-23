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
#include <string.h>
#include <algorithm>
#include <vector>

namespace basis
{
	class bitset
	{
	public:
		bitset()
			:	m_bitCount(0)
		{}

		explicit bitset(size_t count)
			:	m_bitCount(count),
				m_bits(GetNumIntegersForBitcount(count))
		{}

		bitset(int * bits, size_t num_bits)
			:	m_bitCount(num_bits), 
				m_bits(bits, bits + GetNumIntegersForBitcount(num_bits))
		{}

		void set_bit(size_t index, bool b)
		{
			size_t int_index, bit_index;
			bit_index_to_coordinates(index, &int_index, &bit_index);	

			m_bits[int_index] = ((b ? 1 : 0) << bit_index) | (m_bits[int_index] & ~(1 << bit_index));
		}

		void set_or(size_t index, bool b)
		{
			size_t int_index, bit_index;
			bit_index_to_coordinates(index, &int_index, &bit_index);

			m_bits[int_index] = ((b ? 1 : 0) << bit_index) | m_bits[int_index];
		}

		void set_xor(size_t index, bool b)
		{
			size_t int_index, bit_index;
			bit_index_to_coordinates(index, &int_index, &bit_index);

			m_bits[int_index] = ((b ? 1 : 0) << bit_index) ^ m_bits[int_index];
		}

		void set_and(size_t index, bool b)
		{
			size_t int_index, bit_index;
			bit_index_to_coordinates(index, &int_index, &bit_index);

			m_bits[int_index] = ((~0) ^ ((b ? 0 : 1) << bit_index)) & m_bits[int_index];
		}

		void set_all_one()
		{
			memset(&m_bits[0], 0xff, m_bits.size() * sizeof(int));
		}

		void set_all_zero()
		{
			memset(&m_bits[0], 0xff, m_bits.size() * sizeof(int));
		}

		bool test_all_one()
		{ 
			for (size_t i=0, end=m_bits.size(); i<end; i++)
			{
				int v = m_bits[i];
				if (v != (~0))
				{
					if (i == (end - 1))
					{
						size_t leftover = (end * INT_BITS) - m_bitCount;
						int mask = (~0) >> leftover;
						return (v & mask) == mask;
					}
					return false;
				}
			}

			return true;
		}

		bitset operator | (const bitset & b) const
		{
			size_t max_bits = std::max(m_bitCount, b.m_bitCount);
			size_t min_bits = std::min(m_bitCount, b.m_bitCount);
			size_t min_ints = GetNumIntegersForBitcount(min_bits);
			size_t max_ints = GetNumIntegersForBitcount(max_bits);

			int * bits = new int[max_ints];
			for (size_t i=0; i<min_ints; i++)
			{
				bits[i] = m_bits[i] | b.m_bits[i];
			}

			for (size_t i=min_ints; i<max_ints; i++)
			{
				bits[i] = (m_bitCount > b.m_bitCount) ? m_bits[i] : b.m_bits[i];
			}

			return bitset(bits, max_bits);
		}

		bitset operator & (const bitset & b) const
		{
			size_t max_bits = std::max(m_bitCount, b.m_bitCount);
			size_t min_bits = std::min(m_bitCount, b.m_bitCount);
			size_t min_ints = GetNumIntegersForBitcount(min_bits);
			size_t max_ints = GetNumIntegersForBitcount(max_bits);

			int * bits = new int[max_ints];
			memset(bits, 0, sizeof(int) * max_ints);
			for (size_t i=0; i<min_ints; i++)
			{
				bits[i] = m_bits[i] & b.m_bits[i];
			}

			return bitset(bits, max_bits);
		}

		bitset operator ^ (const bitset & b) const
		{
			size_t max_bits = std::max(m_bitCount, b.m_bitCount);
			size_t min_bits = std::min(m_bitCount, b.m_bitCount);
			size_t min_ints = GetNumIntegersForBitcount(min_bits);
			size_t max_ints = GetNumIntegersForBitcount(max_bits);

			int * bits = new int[max_ints];
			for (size_t i=0; i<min_ints; i++)
			{
				bits[i] = m_bits[i] ^ b.m_bits[i];
			}

			for (size_t i=min_ints; i<max_ints; i++)
			{
				bits[i] = (m_bitCount > b.m_bitCount) ? m_bits[i] : b.m_bits[i];
			}

			return bitset(bits, max_bits);
		}

		int operator [] (size_t index) const
		{
			size_t int_index, bit_index;
			bit_index_to_coordinates(index, &int_index, &bit_index);
			return (m_bits[int_index] >> bit_index) & 1;
		}

	private:
		size_t				m_bitCount;
		std::vector<int>	m_bits;

		void bit_index_to_coordinates(size_t bit, size_t * int_index, size_t * bit_index) const
		{
			size_t i = bit / INT_BITS;

			BASIS_ASSERT(i < m_bits.size());

			*int_index = i;
			*bit_index = bit - (i * INT_BITS);
		}

		static size_t GetNumIntegersForBitcount(size_t num_bits)
		{
			return (num_bits + (INT_BITS - 1)) / INT_BITS;
		}

		static const size_t INT_BITS = (sizeof(int) * 8); 
	};
}
 