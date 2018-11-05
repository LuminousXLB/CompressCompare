#include "ioBitHbyte.h"

bool iBitHbyte::getUnit(uint8_t& buf)
{
    return getBits(buf, 4);
}

bool iBitHbyte::getUnits(vector<uint8_t>& buf)
{
    size_t size = buf.size();
    size_t cnt = 0;
    uint8_t b;

    while (cnt < size && getUnit(b)) {
        buf[cnt++] = b;
    }

    buf.resize(cnt);

    return !iBit::eof();
}

void oBitHbyte::putUnit(const uint8_t& buf)
{
    putBits(buf, 4);
}

void oBitHbyte::putUnits(const vector<uint8_t>& buf)
{
    size_t size = buf.size();
    for (size_t i = 0; i < size; i++) {
        putUnit(buf[i]);
    }
}
