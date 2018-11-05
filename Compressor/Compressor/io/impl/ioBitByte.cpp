#include "ioBitByte.h"

bool iBitByte::getUnit(uint8_t& buf)
{
    return getBits(buf, 8);
}

bool iBitByte::getUnits(vector<uint8_t>& buf)
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

void oBitByte::putUnit(const uint8_t& buf)
{
    putBits(buf, 8);
}

void oBitByte::putUnits(const vector<uint8_t>& buf)
{
    size_t size = buf.size();
    for (size_t i = 0; i < size; i++) {
        putUnit(buf[i]);
    }
}
