#pragma once

#include "base.h"

class iBitUnit : public iUnitBase<bool, 2>, protected iBit {
public:
    void open(string filename);

    bool getUnit(bool& buf);
    bool getUnits(vector<bool>& buf);

    bool eof();
};

class oBitUnit : public oUnitBase<bool, 2>, protected oBit {
public:
    void open(string filename);

    void putUnit(const bool& buf);
    void putUnits(const vector<bool>& buf);

    void close();
};
