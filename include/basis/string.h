/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

#include "assert.h"

namespace basis
{
    struct string_header
    {
        uint32_t     length;
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


