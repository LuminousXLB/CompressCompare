#pragma once

#include "base.h"

class ioHbyteUnit {
protected:
    uint8_t CACHE;
    bool cache;

public:
    ioHbyteUnit()
        : CACHE(0)
        , cache(0)
    {
    }
};

class iHbyteUnit : public iUnitBase<uint8_t, 16>, protected ioHbyteUnit {

public:
    bool getUnit(uint8_t& buf);
    bool getUnits(vector<uint8_t>& buf);
};

class oHbyteUnit : public oUnitBase<uint8_t, 16>, protected ioHbyteUnit {
public:
    void putUnit(const uint8_t& buf);
    void putUnits(const vector<uint8_t>& buf);
    void flush();
};
