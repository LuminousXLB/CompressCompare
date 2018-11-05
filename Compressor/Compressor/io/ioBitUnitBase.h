#pragma once

#include "ioBit.h"
#include "ioUnitBase.h"

template <class IntegerType, unsigned USize>
class iBitUnitBase : public iBit, public iUnitBase<IntegerType, USize> {
    static_assert(std::is_integral<IntegerType>::value, "Integral required.");

public:
    void open(string filename)
    {
        iBit::open(filename);
    }

    bool eof()
    {
        return iBit::eof();
    }

    void close()
    {
        iBit::close();
    }
};

template <class IntegerType, unsigned USize>
class oBitUnitBase : public oBit, public oUnitBase<IntegerType, USize> {
    static_assert(std::is_integral<IntegerType>::value, "Integral required.");

public:
    void open(string filename)
    {
#ifdef IOBaseDBG
        __FUNCTION_ENTRANCE__;
#endif // IOBaseDBG

        oBit::open(filename);

#ifdef IOBaseDBG
        __FUNCTION_EXIT__;
#endif // IOBaseDBG
    }

    void close()
    {
        oBit::close();
    }
};
