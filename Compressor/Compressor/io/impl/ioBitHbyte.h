#pragma once

#include "base.h"

class iBitHbyte : public iBitUnitBase<uint8_t, 16> {
public:
    bool getUnit(uint8_t& buf);
    bool getUnits(vector<uint8_t>& buf);
};

class oBitHbyte : public oBitUnitBase<uint8_t, 16> {
public:
    void putUnit(const uint8_t& buf);
    void putUnits(const vector<uint8_t>& buf);
};
