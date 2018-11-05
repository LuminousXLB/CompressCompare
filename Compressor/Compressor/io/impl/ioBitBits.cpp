#include "ioBitBits.h"

bool iBitBits::getUnit(bool& buf)
{
    return iBit::getBit(buf);
}

bool iBitBits::getUnits(vector<bool>& buf)
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

void oBitBits::putUnit(const bool& buf)
{
    oBit::putBit(buf);
}

void oBitBits::putUnits(const vector<bool>& buf)
{
    size_t size = buf.size();
    for (size_t i = 0; i < size; i++) {
        putBit(buf[i]);
    }
}
