#pragma once

#include "common.h"

using namespace std;

class BinaryDecimal {
    vector<bool> data;

    void add(const vector<bool>& ano, size_t off);
    static short compare(const vector<bool>& opl, const vector<bool> opr);

public:
    BinaryDecimal();
    BinaryDecimal(const BinaryDecimal& ano);
    BinaryDecimal(vector<bool> value);
    BinaryDecimal(size_t _count, bool _val);
    BinaryDecimal(double value, size_t bits);

    size_t size() const;
    size_t nextSet(size_t start) const;
    bool operator[](size_t idx) const;

    void add(const BinaryDecimal& ano);
    void mul(const BinaryDecimal& ano);
    void sub(const BinaryDecimal& ano);
    void div(const BinaryDecimal& ano);

    friend BinaryDecimal operator+(const BinaryDecimal& opl, const BinaryDecimal& opr);
    friend BinaryDecimal operator*(const BinaryDecimal& opl, const BinaryDecimal& opr);
    friend bool operator>(const BinaryDecimal& opl, const BinaryDecimal& opr);
    friend bool operator==(const BinaryDecimal& opl, const BinaryDecimal& opr);
    friend bool operator<(const BinaryDecimal& opl, const BinaryDecimal& opr);

    BinaryDecimal& operator=(const BinaryDecimal& ano);
    BinaryDecimal& operator+=(const BinaryDecimal& ano);
    BinaryDecimal& operator*=(const BinaryDecimal& ano);
    BinaryDecimal& operator-=(const BinaryDecimal& ano);
    BinaryDecimal& operator/=(const BinaryDecimal& ano);

    BinaryDecimal operator>>(int off);
    BinaryDecimal& operator>>=(int off);

    void dump(oBit& obb, size_t len);
    void load(iBit& ibb, size_t len);

    void reserve(size_t size);
};
