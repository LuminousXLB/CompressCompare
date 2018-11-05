#pragma once

#include "../structure/Byte.h"
#include "common.h"

using namespace std;

class Buffer {

    array<Byte, 512> buf;

    bool isOutOfRange(int pos)
    {
        return !(pos >= 0 && pos < 4096);
    }

public:
    void* ptr()
    {
        return &buf[0];
    }

    bool get(int pos)
    {
        if (isOutOfRange(pos)) {
            throw out_of_range(string("[ERROR] Out of range caught at ") + string(__FUNCTION__) + string(" with index ") + to_string(pos));
        }
        return buf[pos / 8][pos % 8];
    }

    bool operator[](int pos)
    {
        return get(pos);
    }

    template <class IntegerType>
    int gets(IntegerType& val, int startIdx, int endIdx)
    {
        static_assert(std::is_integral<IntegerType>::value, "Integral required.");
        if (isOutOfRange(startIdx)) {
            throw out_of_range(string("[ERROR] Out of range caught at ") + string(__FUNCTION__) + string(" with startIdx ") + to_string(startIdx));
        }

        val = 0;
        int i = 0;
        for (i = startIdx; i < endIdx && i < 4096; i++) {
            val = (val << 1) + get(i);
        }

        return i - startIdx;
    }

    template <class IntegerType>
    int puts(const IntegerType& val, int startIdx, int endIdx)
    {
        static_assert(std::is_integral<IntegerType>::value, "Integral required.");
        if (isOutOfRange(startIdx)) {
            throw out_of_range(string("[ERROR] Out of range caught at ") + string(__FUNCTION__) + string(" with startIdx ") + to_string(startIdx));
        }

        bitset<64> bs(val);
        int cnt = endIdx - startIdx;

        int i = 0, j = cnt;
        for (i = startIdx; i < endIdx && i < 4096; i++) {
            j--;
            set(i, bs[j]);
        }

        return cnt - j;
    }

    void set(int pos, bool val = 1)
    {
        if (isOutOfRange(pos)) {
            throw out_of_range(string("[ERROR] Out of range caught at ") + string(__FUNCTION__) + string(" with index ") + to_string(pos));
        }
        buf[pos / 8].set(pos % 8, val);
    }

    void clr(int pos)
    {
        set(pos, 0);
    }
};
