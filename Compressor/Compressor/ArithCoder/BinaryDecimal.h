#pragma once

#include "common.h"

using namespace std;

class BinaryDecimal {
    friend bool operator>(const BinaryDecimal& opl, const BinaryDecimal& opr);
    friend bool operator==(const BinaryDecimal& opl, const BinaryDecimal& opr);
    friend bool operator<(const BinaryDecimal& opl, const BinaryDecimal& opr);

    vector<uint8_t> data;
    static short compare(const vector<uint8_t>& opl, const vector<uint8_t>& opr);

public:
    BinaryDecimal();
    BinaryDecimal(int idx);
    BinaryDecimal(const BinaryDecimal& ano);

    static BinaryDecimal add(const BinaryDecimal& accu, int prob);

    BinaryDecimal& add(uint64_t prob);
    BinaryDecimal& shiftAdd(const BinaryDecimal& accu, uint64_t range);
    uint8_t unwrap(const BinaryDecimal& accu, int prob, uint8_t margin);

    size_t size() const;
    size_t shrink();
    void resize(const size_t newsize);

    uint8_t* data_ptr();
};
