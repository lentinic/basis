/*
Chris Lentini
http://divergentcoder.io

This source code is licensed under the MIT license (found in the LICENSE file in the root directory of the project)
*/

#pragma once
#include <atomic>

namespace basis
{
    enum class queue_access_policy
    {
        spsc,
        spmc,
        mpsc,
        mpmc
    };
}
