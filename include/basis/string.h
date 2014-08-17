/*
Copyright (c) 2014 Chris Lentini
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

#include "assert.h"

namespace basis
{
	struct string_header
	{
		uint32_t 	length;
	};

	struct string_rec
	{
		string_header header;
		char data;
	};

	typedef char * string;

	inline string stralloc_len(const char * source, size_t len)
	{
		if (source == nullptr)
		{
			return nullptr;
		}

		BASIS_ASSERT((len & 0xffffffff) == len);
		string_rec * r = (string_rec *) malloc(sizeof(string_rec) + len);
		if (r == nullptr)
		{
			return nullptr;
		}

		r->header.length = (uint32_t)len;
		memcpy(&r->data, source, len);
		(&r->data)[len] = 0;
		return &r->data;
	}

	inline string stralloc(const char * source)
	{
		if (source == nullptr)
		{
			return nullptr;
		}

		size_t len = strlen(source);
		return stralloc_len(source, (uint32_t)len);
	}

	inline string strclone(string other)
	{
		BASIS_ASSERT(other != nullptr);
		uint32_t len = ((string_header *)(other - sizeof(string_header)))->length;
		return stralloc_len(other, len);
	}

	inline const string strempty()
	{
		static const string_rec empty = { 0, 0 };
		return (const string)&empty.data;
	}

	inline void strfree(string str)
	{
		if (str == nullptr)
		{
			return;
		}
		free(str - sizeof(string_header));
	}
}


