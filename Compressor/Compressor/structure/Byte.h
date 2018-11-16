#pragma once

#include "common.h"

using namespace std;

class Byte {
    struct {
        bool x07 : 1;
        bool x06 : 1;
        bool x05 : 1;
        bool x04 : 1;
        bool x03 : 1;
        bool x02 : 1;
        bool x01 : 1;
        bool x00 : 1;
    } b_;

public:
    Byte();
    Byte(const uint8_t& val);
    Byte(const array<bool, 8>& val);

    bool get(int pos) const;
    bool operator[](int pos) const;

    void set(int pos, bool val = 1);
    void clr(int pos);
    void clear();
    void flip(int pos);

    uint8_t to_int() const;
    string toString() const;
};
