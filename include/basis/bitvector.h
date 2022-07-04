/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

#include <stdint.h>
#include <string.h>
#include <algorithm>
#include <vector>

#include "assert.h"

namespace basis
{
    class bitvector
    {
    public:
        bitvector()
        :   m_bitCount(0)
        {}

        explicit bitvector(size_t count)
        :   m_bitCount(count),
            m_bits(GetNumIntegersForBitcount(count))
        {}

        bitvector(uint32_t * bits, size_t num_bits)
        :   m_bitCount(num_bits), 
            m_bits(bits, bits + GetNumIntegersForBitcount(num_bits))
        {}

        void push_back(bool b)
        {
            size_t num_bits = m_bitCount++;
            size_t num_ints = m_bits.size();
            if (GetNumIntegersForBitcount(num_bits + 1) > num_ints)
            {
                m_bits.push_back(b ? 1 : 0);
            }    
            else
            {
                set_bit(num_bits, b);
            }
        }

        bool pop_back()
        {
            BASIS_ASSERT(m_bitCount > 0);

            size_t num_bits = m_bitCount--;
            bool r = get_bit(num_bits - 1);
            if ((num_bits % INT_BITS) == 1)
            {
                m_bits.pop_back();
            }
            return r;
        }

        size_t count() const
        {
            return m_bitCount;
        }

        bool get_bit(size_t index) const
        {
            size_t int_index, bit_index;
            bit_index_to_coordinates(index, &int_index, &bit_index);
            return (m_bits[int_index] >> bit_index) & 1;
        }

        void set_bit(size_t index, bool b)
        {
            size_t int_index, bit_index;
            bit_index_to_coordinates(index, &int_index, &bit_index);    

            m_bits[int_index] = ((b ? 1 : 0) << bit_index) | (m_bits[int_index] & ~(1 << bit_index));
        }

        void toggle_bit(size_t index)
        {
            size_t int_index, bit_index;
            bit_index_to_coordinates(index, &int_index, &bit_index);    

            m_bits[int_index] ^= (1 << bit_index);    
        }

        void set_all_one()
        {
            memset(&m_bits[0], 0xff, m_bits.size() * sizeof(int));
        }

        void set_all_zero()
        {
            memset(&m_bits[0], 0, m_bits.size() * sizeof(int));
        }

        bool test_all(bool b)
        { 
            uint32_t testVal = b ? ~0 : 0;
            size_t last = m_bitCount / INT_BITS;
            
            BASIS_ASSERT(last <= m_bits.size());

            for (size_t i=0; i<last; i++)
            {
                if (m_bits[i] != testVal)
                {
                    return false;
                }
            }

            uint32_t remainder = m_bitCount & 31;
            if (remainder != 0) {
                uint32_t mask = (1 << remainder) - 1;
                return (m_bits[last + 1] & mask) == (testVal & mask);
            }
            return true;
        }

        bitvector operator | (const bitvector & b) const
        {
            size_t max_bits = std::max(m_bitCount, b.m_bitCount);
            size_t min_bits = std::min(m_bitCount, b.m_bitCount);
            size_t min_ints = GetNumIntegersForBitcount(min_bits);
            size_t max_ints = GetNumIntegersForBitcount(max_bits);

            uint32_t * bits = new uint32_t[max_ints];
            for (size_t i=0; i<min_ints; i++)
            {
                bits[i] = m_bits[i] | b.m_bits[i];
            }

            for (size_t i=min_ints; i<max_ints; i++)
            {
                bits[i] = (m_bitCount > b.m_bitCount) ? m_bits[i] : b.m_bits[i];
            }

            return bitvector(bits, max_bits);
        }

        bitvector operator & (const bitvector & b) const
        {
            size_t max_bits = std::max(m_bitCount, b.m_bitCount);
            size_t min_bits = std::min(m_bitCount, b.m_bitCount);
            size_t min_ints = GetNumIntegersForBitcount(min_bits);
            size_t max_ints = GetNumIntegersForBitcount(max_bits);

            uint32_t * bits = new uint32_t[max_ints];
            memset(bits, 0, sizeof(int) * max_ints);
            for (size_t i=0; i<min_ints; i++)
            {
                bits[i] = m_bits[i] & b.m_bits[i];
            }

            return bitvector(bits, max_bits);
        }

        bitvector operator ^ (const bitvector & b) const
        {
            size_t max_bits = std::max(m_bitCount, b.m_bitCount);
            size_t min_bits = std::min(m_bitCount, b.m_bitCount);
            size_t min_ints = GetNumIntegersForBitcount(min_bits);
            size_t max_ints = GetNumIntegersForBitcount(max_bits);

            uint32_t * bits = new uint32_t[max_ints];
            for (size_t i=0; i<min_ints; i++)
            {
                bits[i] = m_bits[i] ^ b.m_bits[i];
            }

            for (size_t i=min_ints; i<max_ints; i++)
            {
                bits[i] = (m_bitCount > b.m_bitCount) ? m_bits[i] : b.m_bits[i];
            }

            return bitvector(bits, max_bits);
        }

        bool operator [] (size_t index) const
        {
            return get_bit(index);
        }

    private:
        size_t                  m_bitCount;
        std::vector<uint32_t>   m_bits;

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

        static constexpr size_t INT_BITS = 32;
    };
}
 