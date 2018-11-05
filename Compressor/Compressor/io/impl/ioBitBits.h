#pragma once

#include "base.h"

class iBitBits : public iBitUnitBase<bool, 2> {
public:
    bool getUnit(bool& buf);
    bool getUnits(vector<bool>& buf);
};

class oBitBits : public oBitUnitBase<bool, 2> {
public:
    void putUnit(const bool& buf);
    void putUnits(const vector<bool>& buf);
};
