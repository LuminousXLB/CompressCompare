#pragma once

#include "common.h"
#include "ioBase.h"

using namespace std;

class ioUnitBase {
public:
};

template <class IntegerType, unsigned USize>
class iUnitBase : public iBase, public ioUnitBase {
    static_assert(std::is_integral<IntegerType>::value, "Integral required.");

public:
    virtual void open(string filename)
    {
        f.open(filename);
    }

    virtual bool getUnit(IntegerType& buf) = 0;
    virtual bool getUnits(vector<IntegerType>& buf) = 0;

    virtual bool eof()
    {
        return f.eof();
    }

    virtual void close()
    {
        f.close();
    }
};

template <class IntegerType, unsigned USize>
class oUnitBase : public oBase, public ioUnitBase {
    static_assert(std::is_integral<IntegerType>::value, "Integral required.");

public:
    virtual void open(string filename)
    {
        f.open(filename);
    }

    virtual void putUnit(const IntegerType& buf) = 0;
    virtual void putUnits(const vector<IntegerType>& buf) = 0;

    virtual void close()
    {
        f.close();
    }
};
