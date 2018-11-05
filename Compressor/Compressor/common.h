#pragma once

#include "dbgsettings.h"
#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <system_error>
#include <type_traits>
#include <utility>
#include <vector>

template <class IntegerType>
IntegerType ceilingDiv(IntegerType dividend, IntegerType divisor)
{
    static_assert(is_integral<IntegerType>::value, "Integral required.");

    return dividend / divisor + ((dividend % divisor == 0) ? 0 : 1);
}

template <class IntegerType>
unsigned lb(IntegerType data)
{
    static_assert(is_integral<IntegerType>::value, "Integral required.");

    unsigned cnt = 0;

    while (data > 1) {
        data >>= 1;
        cnt++;
    }

    return cnt;
}
