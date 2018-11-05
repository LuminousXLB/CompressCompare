#pragma once

#include "common.h"

template <class IntegerType, unsigned USize>
class ioLZEncoder {
    iUnitBase<IntegerType, USize>* ifp;
    oBitUnitBase<IntegerType, USize>* ofp;

public:
    ioLZEncoder()
        : ifp(nullptr)
        , ofp(nullptr)
    {
    }

    void open(iUnitBase<IntegerType, USize>* _ifp, string infile, oBitUnitBase<IntegerType, USize>* _ofp, string outfile)
    {
        ifp = _ifp;
        ofp = _ofp;

        ifp->open(infile);
        ofp->open(outfile);
    }

    bool is_open()
    {
        return ifp->is_open() && ofp->is_open();
    }

    bool eof()
    {
        return ifp->eof();
    }

    bool getUnit(IntegerType& buf)
    {
        return ifp->getUnit(buf);
    }

    void putUnit(const IntegerType& buf)
    {
        ofp->putUnit(buf);
    }

    void putBits(const size_t& buf, uint8_t len)
    {
        ofp->putBits(buf, len);
    }

    void dumpLength()
    {
        ofp->dumpLength();
    }

    void close()
    {
        if (ifp) {
            ifp->close();
            delete ifp;
            ifp = nullptr;
        }
        if (ofp) {
            ofp->close();
            delete ofp;
            ofp = nullptr;
        }
    }

    ~ioLZEncoder()
    {
        close();
    }
};

template <class IntegerType, unsigned USize>
class ioLZDecoder {
    iBitUnitBase<IntegerType, USize>* ifp;
    oUnitBase<IntegerType, USize>* ofp;

public:
    ioLZDecoder()
        : ifp(nullptr)
        , ofp(nullptr)
    {
    }

    void open(iBitUnitBase<IntegerType, USize>* _ifp, string infile, oUnitBase<IntegerType, USize>* _ofp, string outfile)
    {
        ifp = _ifp;
        ofp = _ofp;

        ifp->open(infile);
        ofp->open(outfile);
    }

    bool is_open()
    {
        return ifp->is_open() && ofp->is_open();
    }

    bool eof()
    {
        return ifp->eof();
    }

    uint8_t getBits(size_t& buf, uint8_t len)
    {
        return ifp->getBits(buf, len);
    }

    bool getUnit(IntegerType& buf)
    {
        return ifp->getUnit(buf);
    }

    void putUnits(const vector<IntegerType>& buf)
    {
        ofp->putUnits(buf);
    }

    size_t extractLength()
    {
        return ifp->extractLength();
    }

    void close()
    {
        if (ifp) {
            ifp->close();
            delete ifp;
            ifp = nullptr;
        }
        if (ofp) {
            ofp->close();
            delete ofp;
            ofp = nullptr;
        }
    }

    ~ioLZDecoder()
    {
        close();
    }
};
