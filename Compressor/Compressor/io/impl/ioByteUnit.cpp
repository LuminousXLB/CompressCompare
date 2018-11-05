#include "ioByteUnit.h"

bool iByteUnit::getUnit(uint8_t& buf)
{
    f.read(&buf, 1, 1, 1);
    return !f.eof();
}

bool iByteUnit::getUnits(vector<uint8_t>& buf)
{
    size_t cnt = f.read(&buf[0], buf.capacity(), 1, buf.size());

    buf.resize(cnt);

    return !f.eof();
}

void oByteUnit::putUnit(const uint8_t& buf)
{
    f.write(&buf, 1, 1);
}

void oByteUnit::putUnits(const vector<uint8_t>& buf)
{
    f.write(&buf[0], 1, buf.size());
}
