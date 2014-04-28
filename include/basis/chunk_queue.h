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
#include <atomic>
#include "thread_util.h"
#include "ring_queue.h"
#include "queue_access_policy.h"
#include "assert.h"

// TODO: Manage chunk transitions a little more gracefully... For the multiple
// consumer/producer cases we are just spinlocking on chunk transitions.

namespace basis
{
	template<class TYPE, queue_access_policy ACCESS>
	class chunk_queue;

	namespace internal
	{
		template<class TYPE, queue_access_policy ACCESS>
		struct chunk_block
		{
			chunk_block(uint32_t size)
				:	ring(size),
					next(nullptr)
			{}

			static chunk_block * lock_acquire(std::atomic<chunk_block *> & node, std::atomic<uint32_t> & lock)
			{
				lock.fetch_add(1, std::memory_order_relaxed);
				auto r = node.load(std::memory_order_acquire);
				for (; !r; r = node.load(std::memory_order_acquire))
				{
					lock.fetch_sub(1, std::memory_order_relaxed);
					cpu_yield();
					lock.fetch_add(1, std::memory_order_relaxed);
				}
				return r;
			}

			static void lock_release(std::atomic<uint32_t> & lock)
			{
				lock.fetch_sub(1, std::memory_order_relaxed);
			}

			static void wait_for_unlocked(std::atomic<uint32_t> & lock)
			{
				while (lock.load(std::memory_order_relaxed) > 0)
					cpu_yield();
			}

			ring_queue<TYPE,ACCESS>		ring;
			std::atomic<chunk_block *>	next;
		};
	}

	template<class TYPE>
	class chunk_queue<TYPE, queue_access_policy::spsc>
	{
	public:
		explicit chunk_queue(uint32_t chunk_size)
			:	m_head(new chunk_block(chunk_size)),
				m_tail(m_head),
				m_chunkSize(chunk_size)
		{}

		template<class U>
		void push_back(U && data)
		{
			if (!m_tail->ring.push_back(data))
			{
				auto n = new chunk_block(m_chunkSize);
				n->ring.push_back(data);
				m_tail->next.store(n, std::memory_order_release);
				m_tail = n;
			}
		}

		bool pop_front(TYPE & dest)
		{
			auto f = m_head;
			if (!f->ring.pop_front(dest))
			{
				auto n = f->next.load(std::memory_order_acquire);
				if (!n)
				{
					return false;
				}

				if (f->ring.pop_front(dest))
				{
					return true;
				}

				// if we have a next node, something must be there...
				bool success = n->ring.pop_front(dest);
				BASIS_ASSERT(success);
				m_head = n;
				delete f;
			}	
			return true;
		}

	private:
		chunk_queue(const chunk_queue &) = delete;
		chunk_queue & operator = (const chunk_queue &) = delete;

		typedef internal::chunk_block<TYPE,queue_access_policy::spsc> chunk_block;

		chunk_block * 	m_head;
		chunk_block *	m_tail;
		const uint32_t	m_chunkSize;
	};

	template<class TYPE>
	class chunk_queue<TYPE, queue_access_policy::spmc>
	{
	public:
		explicit chunk_queue(uint32_t chunk_size)
			:	m_lockHead(0),
				m_head(new chunk_block(chunk_size)),
				m_tail(m_head),
				m_chunkSize(chunk_size)
		{}

		template<class U>
		void push_back(U && data)
		{
			auto tail = m_tail;
			if (!tail->ring.push_back(data))
			{
				auto n = new chunk_block(m_chunkSize);
				n->ring.push_back(data);
				tail->next.store(n, std::memory_order_release);
				m_tail = n;
			}
		}

		bool pop_front(TYPE & dest)
		{
			auto head = chunk_block::lock_acquire(m_head, m_lockHead);
			if (!head->ring.pop_front(dest))
			{
				auto next = head->next.load();
				if (!next)
				{
					chunk_block::lock_release(m_lockHead);
					return false;
				}

				if (head->ring.pop_front(dest))
				{
					chunk_block::lock_release(m_lockHead);
					return true;
				}

				if (!next->ring.pop_front(dest))
				{
					chunk_block::lock_release(m_lockHead);
					return false;
				}

				chunk_block::lock_release(m_lockHead);
				if (m_head.compare_exchange_strong(head, nullptr))
				{
					chunk_block::wait_for_unlocked(m_lockHead);
					m_head.store(next);
					delete head;
				}

				return true;
			}
			chunk_block::lock_release(m_lockHead);
			return true;
		}

	private:
		chunk_queue(const chunk_queue &) = delete;
		chunk_queue & operator = (const chunk_queue &) = delete;

		typedef internal::chunk_block<TYPE, queue_access_policy::spmc> chunk_block;

		std::atomic<uint32_t>		m_lockHead;
		std::atomic<chunk_block *>	m_head;
		chunk_block *				m_tail;
		const uint32_t				m_chunkSize;
	};
	
	template<class TYPE>
	class chunk_queue<TYPE, queue_access_policy::mpsc>
	{
	public:
		explicit chunk_queue(uint32_t chunk_size)
			:	m_lockTail(0),
				m_tail(new chunk_block(chunk_size)),
				m_head(m_tail.load()),
				m_chunkSize(chunk_size)
		{}

		template<class U>
		void push_back(U && data)
		{
			auto tail = chunk_block::lock_acquire(m_tail, m_lockTail);
			while (!tail->ring.push_back(data))
			{
				if (m_tail.compare_exchange_strong(tail, nullptr, std::memory_order_relaxed))
				{
					chunk_block::lock_release(m_lockTail);

					auto next = new chunk_block(m_chunkSize);
					bool success = next->ring.push_back(data);
					BASIS_ASSERT(success);

					chunk_block::wait_for_unlocked(m_lockTail);

					tail->next.store(next, std::memory_order_release);
					m_tail.store(next, std::memory_order_release);
					return;
				}
				else if (tail == nullptr)
				{
					chunk_block::lock_release(m_lockTail);
					tail = chunk_block::lock_acquire(m_tail, m_lockTail);
				}

			}
			chunk_block::lock_release(m_lockTail);
		}

		bool pop_front(TYPE & dest)
		{
			auto head = m_head;
			if (!head->ring.pop_front(dest))
			{
				auto next = head->next.load(std::memory_order_acquire);
				if (!next)
				{
					return false;
				}

				if (head->ring.pop_front(dest))
				{
					return true;
				}

				// if we have a next node, something must be there...
				bool success = next->ring.pop_front(dest);
				BASIS_ASSERT(success);
				m_head = (chunk_block *) next;
				delete head;
			}	
			return true;
		}

	private:
		chunk_queue(const chunk_queue &) = delete;
		chunk_queue & operator = (const chunk_queue &) = delete;

		typedef internal::chunk_block<TYPE, queue_access_policy::mpsc> chunk_block;

		std::atomic<uint32_t>		m_lockTail;
		std::atomic<chunk_block *>	m_tail;
		chunk_block *				m_head;
		uint32_t					m_chunkSize;
	};

	template<class TYPE>
	class chunk_queue<TYPE, queue_access_policy::mpmc>
	{
	public:
		explicit chunk_queue(uint32_t chunk_size)
			:	m_lockTail(0),
				m_lockHead(0),
				m_head(new chunk_block(chunk_size)),
				m_tail(m_head.load()),
				m_chunkSize(chunk_size)
		{}

		template<class U>
		void push_back(U && data)
		{
			auto tail = chunk_block::lock_acquire(m_tail, m_lockTail);
			while (!tail->ring.push_back(data))
			{
				if (m_tail.compare_exchange_strong(tail, nullptr, std::memory_order_relaxed))
				{
					chunk_block::lock_release(m_lockTail);

					auto next = new chunk_block(m_chunkSize);
					bool success = next->ring.push_back(data);
					BASIS_ASSERT(success);

					chunk_block::wait_for_unlocked(m_lockTail);

					tail->next.store(next, std::memory_order_release);
					m_tail.store(next, std::memory_order_release);
					return;
				}
				else if (tail == nullptr)
				{
					chunk_block::lock_release(m_lockTail);
					tail = chunk_block::lock_acquire(m_tail, m_lockTail);
				}
			}
			chunk_block::lock_release(m_lockTail);
		}

		bool pop_front(TYPE & dest)
		{
			auto head = chunk_block::lock_acquire(m_head, m_lockHead);
			if (!head->ring.pop_front(dest))
			{
				auto next = head->next.load(std::memory_order_acquire);
				if (!next)
				{
					chunk_block::lock_release(m_lockHead);
					return false;
				}

				if (head->ring.pop_front(dest))
				{
					chunk_block::lock_release(m_lockHead);
					return true;
				}

				if (!next->ring.pop_front(dest))
				{
					chunk_block::lock_release(m_lockHead);
					return false;
				}

				chunk_block::lock_release(m_lockHead);
				if (m_head.compare_exchange_strong(head, nullptr, std::memory_order_relaxed))
				{
					chunk_block::wait_for_unlocked(m_lockHead);
					m_head.store(next, std::memory_order_release);
					delete head;
				}

				return true;
			}
			chunk_block::lock_release(m_lockHead);
			return true;
		}

	private:
		chunk_queue(const chunk_queue &) = delete;
		chunk_queue & operator = (const chunk_queue &) = delete;

		typedef internal::chunk_block<TYPE, queue_access_policy::mpmc> chunk_block;

		std::atomic<uint32_t>		m_lockTail;
		std::atomic<uint32_t>		m_lockHead;
		std::atomic<chunk_block *>	m_head;
		std::atomic<chunk_block *>	m_tail;
		const uint32_t				m_chunkSize;
	};
}
