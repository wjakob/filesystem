/*
    std_to_string.h -- A simple class for manipulating paths on Linux/Windows/Mac OS

    Copyright (c) 2019 Wenzel Jakob <wenzel@inf.ethz.ch>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/
#pragma once

#if (defined(_MSC_VER) && (_MSC_VER < 1700)) || (__cplusplus < 201103L)
#define CUSTOM_STD_TO_STING (1)
#include <string>
#include <cstdio>
namespace std {
std::string to_string(unsigned long i)
{
    char buf[256];
    _snprintf(buf, 256, "%lu", i);
    return std::string(buf);
}
} // namespace std

#endif
