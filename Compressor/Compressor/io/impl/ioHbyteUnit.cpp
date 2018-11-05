#include "ioHbyteUnit.h"

bool iHbyteUnit::getUnit(uint8_t& buf)
{
    if (!f.eof()) {
        if (!cache) {
            f.read(&CACHE, 1, 1, 1);
            buf = (CACHE >> 4) & 0xf;
            cache = true;
            return !f.eof();
        } else {
            buf = CACHE & 0xf;
            cache = false;
            return true;
        }
    } else {
        return false;
    }
}

bool iHbyteUnit::getUnits(vector<uint8_t>& buf)
{
    size_t size = buf.size();
    size_t cnt = 0;
    uint8_t b;
    bool state;

    while (cnt < size && (state = getUnit(b))) {
        buf[cnt++] = b;
    }

    buf.resize(cnt);

    return state;
}

void oHbyteUnit::putUnit(const uint8_t& buf)
{
    if (cache) {
        CACHE += buf;
        flush();
    } else {
        CACHE = buf;
        CACHE <<= 4;
        cache = true;
    }
}

void oHbyteUnit::putUnits(const vector<uint8_t>& buf)
{
    size_t size = buf.size();
    for (size_t i = 0; i < size; i++) {
        putUnit(buf[i]);
    }
}

void oHbyteUnit::flush()
{
    if (cache) {
        cache = false;
        f.write(&CACHE, 1, 1);
        CACHE = 0;
    }
}
