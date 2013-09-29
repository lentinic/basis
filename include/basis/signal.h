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
		typedef std::function<void(PARAMETERS...)> function_type;

		uint32_t connect(function_type fn)
		{
			return m_listeners.add(fn);
		}

		void disconnect(uint32_t id)
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
		object_table<function_type> m_listeners;
	};
}
