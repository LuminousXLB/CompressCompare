#pragma once

#include "Buffer.h"
#include "common.h"
#include "ioBase.h"

using namespace std;

const unsigned BUFFER_SIZE_BYTE = 512;
const unsigned BUFFER_SIZE_BIT = 8 * BUFFER_SIZE_BYTE;

///////////////////////////////////////////////////////////////////////////////

class ioBit {
protected:
    Buffer buffer;
    unsigned pos;
    size_t count;

public:
    virtual size_t flush() = 0;

    size_t getCount()
    {
        return count;
    }
};

///////////////////////////////////////////////////////////////////////////////

class iBit : public iBase, protected ioBit {
    size_t bit_len;

public:
    void open(string filename);

    bool eof() const;
    size_t length() const;

    size_t flush();

    bool getBit(bool& buf);

    template <class IntegerType>
    uint8_t getBits(IntegerType& buf, uint8_t len);

    long tell();
    void seek(long offset, int origin);

    size_t extractLength();
};

///////////////////////////////////////////////////////////////////////////////

class oBit : public oBase, protected ioBit {
public:
    void open(string filename);

    size_t flush();

    void putBit(const bool& buf);

    template <class IntegerType>
    void putBits(const IntegerType& buf, uint8_t len);

    void dumpLength();

    void close();

    ~oBit();
};

///////////////////////////////////////////////////////////////////////////////

template <class IntegerType>
inline uint8_t iBit::getBits(IntegerType& buf, uint8_t len)
{
    static_assert(std::is_integral<IntegerType>::value, "Integral required.");
    uint8_t cnt = 0;

    buf = 0;

    if (!eof()) {
        if (len > 8 * sizeof(IntegerType)) {
            throw length_error("[ERROR] Attempting to get too many bits");
        }

        while (cnt < len && !eof()) {
            if (pos >= BUFFER_SIZE_BIT) {
                flush();
            }

            IntegerType O_o = 0;
            uint8_t off = buffer.gets(O_o, pos, pos + len - cnt);

            buf = (buf << off) + O_o;
            cnt += off;
            pos += off;
            count += off;
        }
    }

    if (eof()) {
        uint8_t margin = uint8_t(count - bit_len);
        return cnt - margin;
    } else {
        return cnt;
    }
}

template <class IntegerType>
inline void oBit::putBits(const IntegerType& buf, uint8_t len)
{
    static_assert(std::is_integral<IntegerType>::value, "Integral required.");
    if (len > 8 * sizeof(IntegerType)) {
        throw length_error("[ERROR] Attempting to put too many bits");
    }

    uint8_t cnt = 0;

    while (cnt < len) {
        if (pos >= BUFFER_SIZE_BIT) {
            flush();
        }

        uint8_t off = buffer.puts(buf, pos, pos + len - cnt);

        cnt += off;
        pos += off;
        count += off;
    }
}
