#include "ioBit.h"

void iBit::open(string filename)
{
    f.open(filename);

    if (f.is_open()) {
        bit_len = f.length() * 8;
        pos = 0;
        count = 0;
    }

    flush();
}

bool iBit::eof() const
{
    return count > bit_len;
}

size_t iBit::length() const
{
    return bit_len;
}

size_t iBit::flush()
{
    if (f.is_open()) {
        size_t cnt = f.read(buffer.ptr(), sizeof(buffer), sizeof(Byte), BUFFER_SIZE_BYTE);

        f.checkError();

        pos = 0;

        return cnt;
    } else {
        return 0;
    }
}

bool iBit::getBit(bool& buf)
{
    if (!eof()) {
        if (pos >= BUFFER_SIZE_BIT) {
            flush();
        }

        buf = buffer[pos++];
        count++;
    }

    return !eof();
}

long iBit::tell()
{
    return f.tell();
}

void iBit::seek(long offset, int origin)
{
    f.seek(offset, origin);
}

size_t iBit::extractLength()
{
    if (f.is_open()) {
        uint64_t length;

        long fptr = f.tell();

        f.seek(-8l, SEEK_END);
        f.read(&length, sizeof(length), sizeof(length), 1);
        f.seek(fptr, SEEK_SET);

        bit_len = size_t(length);

        return bit_len;
    }

    return 0;
}

void oBit::open(string filename)
{
    f.open(filename);

    if (f.is_open()) {
        pos = 0;
        count = 0;
    }
}

size_t oBit::flush()
{
    if (f.is_open() && pos > 0) {
        unsigned cnt = ceilingDiv(pos, 8u);

        cnt = f.write(buffer.ptr(), sizeof(Byte), cnt);
        f.checkError();

        pos = 0;

        return cnt;
    } else {
        return 0;
    }
}

void oBit::putBit(const bool& buf)
{
    if (pos >= BUFFER_SIZE_BIT) {
        flush();
    }

    buffer.set(pos++, buf);
    count++;
}

void oBit::dumpLength()
{
    flush();

    uint64_t length = count;
    f.write(&length, sizeof(length), 1);
}

void oBit::close()
{
    flush();
    oBase::close();
}

oBit::~oBit()
{
    flush();
}
