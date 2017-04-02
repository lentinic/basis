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

#define BASIS_MACRO_BLOCK(macro) do { \
        macro \
    } while((void)0, 0)

#define BASIS_UNUSED_1(_1) BASIS_MACRO_BLOCK( \
        (void)(true ? (void)0 : ((void)(_1))); \
    )
#define BASIS_UNUSED_2(_1, _2) BASIS_UNUSED_1(_1); BASIS_UNUSED_1(_2)
#define BASIS_UNUSED_3(_1, _2, _3) BASIS_UNUSED_1(_1); BASIS_UNUSED_2(_2, _3)
#define BASIS_UNUSED_4(_1, _2, _3, _4) BASIS_UNUSED_1(_1); BASIS_UNUSED_3(_2, _3, _4)
#define BASIS_UNUSED_5(_1, _2, _3, _4, _5) BASIS_UNUSED_1(_1); BASIS_UNUSED_4(_2, _3, _4, _5)
#define BASIS_UNUSED_6(_1, _2, _3, _4, _5, _6) BASIS_UNUSED_1(_1); BASIS_UNUSED_5(_2, _3, _4, _5, _6)
#define BASIS_UNUSED_7(_1, _2, _3, _4, _5, _6, _7) BASIS_UNUSED_1(_1); BASIS_UNUSED_6(_2, _3, _4, _5, _6, _7)
#define BASIS_UNUSED_8(_1, _2, _3, _4, _5, _6, _7, _8) BASIS_UNUSED_1(_1); BASIS_UNUSED_7(_2, _3, _4, _5, _6, _7, _8)

#define BASIS_UNUSED_SELECTOR(tuple) BASIS_UNUSED_SELECTOR_IMPL tuple
#define BASIS_UNUSED_SELECTOR_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,N,...) N
#define BASIS_UNUSED(...) BASIS_UNUSED_SELECTOR((__VA_ARGS__,BASIS_UNUSED_8,BASIS_UNUSED_7,BASIS_UNUSED_6, \
    BASIS_UNUSED_5, BASIS_UNUSED_4, BASIS_UNUSED_3, BASIS_UNUSED_2, BASIS_UNUSED_1))(__VA_ARGS__)

#define BASIS_CONCAT_BASE(x,y) x ## y
#define BASIS_CONCAT(x,y) BASIS_CONCAT_BASE(x,y)
