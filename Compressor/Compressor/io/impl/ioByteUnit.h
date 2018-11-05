#pragma once

#include "base.h"

class iByteUnit : public iUnitBase<uint8_t, 256> {
public:
    bool getUnit(uint8_t& buf);
    bool getUnits(vector<uint8_t>& buf);
};

class oByteUnit : public oUnitBase<uint8_t, 256> {
public:
    void putUnit(const uint8_t& buf);
    void putUnits(const vector<uint8_t>& buf);
};
