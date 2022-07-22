/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "basis/string.h"
#include "basis/assert.h"

namespace basis
{
    struct string_header
    {
        uint32_t     length;
        uint32_t     capacity;
        char         data[];
    };

    static string_header * strheader(string s)
    {
        return (string_header *)(s - sizeof(string_header));
    }

    string stralloc(const char * source, size_t len)
    {
        BASIS_ASSERT((len & 0xffffffff) == len);

        size_t hsz = sizeof(string_header);
        string_header * hdr = (string_header *) malloc(hsz + len + 1);
        if (hdr == nullptr)
        {
            return nullptr;
        }

        hdr->length = (uint32_t) len;
        hdr->capacity = (uint32_t) len;
        if (source && len)
        {
            memcpy(hdr->data, source, len);
        }
        hdr->data[len] = 0;
        return hdr->data;
    }

    string stralloc(const char * source)
    {
        size_t len = source ? strlen(source) : 0;
        return stralloc(source, len);
    }

    string strempty()
    {
        return stralloc("", 0);
    }

    string strclone(string other)
    {
        BASIS_ASSERT(other != nullptr);
        string_header * hdr = strheader(other);
        return stralloc(other, hdr->length);
    }

    void strfree(string str)
    {
        if (str == nullptr)
        {
            return;
        }
        free(strheader(str));
    }

    string strvfmt(const char * fmt, va_list params)
    {
        char stackbuf[256];
        size_t bufsz = sizeof(stackbuf);

        va_list tmp;
        va_copy(tmp, params);
        int r = vsnprintf(stackbuf, bufsz, fmt, tmp);
        va_end(tmp);
        if (r < 0)
        {
            return nullptr;
        }
        else if (r < bufsz)
        {
            return stralloc(stackbuf, r);
        }

        string s = stralloc(nullptr, r);
        r = vsnprintf(s, r, fmt, params);
        if (r < 0)
        {
            strfree(s);
            return nullptr;
        }

        return s;
    }
}