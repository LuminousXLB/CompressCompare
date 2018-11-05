#include "ioBitUnit.h"

void iBitUnit::open(string filename)
{
    iBit::open(filename);
}

bool iBitUnit::getUnit(bool& buf)
{
    return iBit::getBit(buf);
}

bool iBitUnit::getUnits(vector<bool>& buf)
{
    size_t size = buf.size();
    size_t cnt = 0;
    bool b;
    while (cnt < size && getBit(b)) {
        buf[cnt++] = b;
    }

    buf.resize(cnt);

    return !iBit::eof();
}

bool iBitUnit::eof()
{
    return iBit::eof();
}

void oBitUnit::open(string filename)
{
    oBit::open(filename);
}

void oBitUnit::putUnit(const bool& buf)
{
    oBit::putBit(buf);
}

void oBitUnit::putUnits(const vector<bool>& buf)
{
    size_t size = buf.size();
    for (size_t i = 0; i < size; i++) {
        putBit(buf[i]);
    }
}

void oBitUnit::close()
{
    oBit::close();
}
