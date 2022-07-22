/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once

namespace basis
{
    typedef char * string;

    string  stralloc(const char * source, size_t len);
    string  stralloc(const char * source);
    string  strempty();
    string  strclone(string other);
    void    strfree(string str);

    string  strvfmt(const char * fmt, va_list params);
}


