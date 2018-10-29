#include "bitstream.h"

size_t ibstream::flush()
{
    if (is_open()) {

        size_t cnt = fread(buffer, sizeof(Byte), BUFFER_SIZE_BYTE, fp);

        int base = 0;
        for (size_t i = 0; i < cnt; i++) {
            for (int j = 0; j < 8; j++) {
                bits[base + j] = Bit(buffer[i] & bit_mask[j]);
            }
            base += 8;
        }

        pos = 0;

        D_PFERROR_;

        return cnt;
    } else {
        return 0;
    }
}

ibstream::ibstream()
{
}

ibstream::ibstream(char const* filename)
    : bstream(filename, true)
{
    if (fp) {
        fseek(fp, 0L, SEEK_END);
        file_length = ftell(fp) * 8;
        fseek(fp, 0L, SEEK_SET);
    } else {
        file_length = -1;
    }

    flush();
}

ibstream::~ibstream()
{
}

void ibstream::open(char const* filename)
{
    bstream::open(filename, true);

    if (fp) {
        fseek(fp, 0L, SEEK_END);
        file_length = ftell(fp) * 8;
        fseek(fp, 0L, SEEK_SET);
    } else {
        file_length = -1;
    }

    flush();
}

bool ibstream::getb(Bit& b)
{
    if (!b_eof()) {
        if (pos >= BUFFER_SIZE_BIT) {
            flush();
        }

        b = bits[pos++];
        count++;
    }

    return !b_eof();
}

size_t ibstream::gets(Bit*& buffer, size_t cnt)
{

    if (buffer != nullptr) {
        delete[] buffer;
    }

    buffer = new Bit[cnt];

    Bit* ptr = buffer;
    size_t bmargin = BUFFER_SIZE_BIT - pos;

    if (pos >= BUFFER_SIZE_BIT) {
        bmargin = flush();
    }

    if (cnt > bmargin && !f_eof()) {
        memcpy_s(ptr, cnt, bits + pos, bmargin);
        ptr += bmargin;
        cnt -= bmargin;
        pos += bmargin;
        count += bmargin;
        bmargin = flush();
    }

    while (cnt > BUFFER_SIZE_BIT && !f_eof()) {
        memcpy_s(ptr, cnt, bits, BUFFER_SIZE_BIT);
        ptr += BUFFER_SIZE_BIT;
        cnt -= BUFFER_SIZE_BIT;
        count += BUFFER_SIZE_BIT;
        bmargin = flush();
    }

    if (cnt <= bmargin) {
        memcpy_s(ptr, cnt, bits + pos, cnt);
        ptr += cnt;
        pos += cnt;
        count += cnt;
    } else if (f_eof()) {
        memcpy_s(ptr, cnt, bits + pos, bmargin);
        ptr += bmargin;
        pos += bmargin;
        count += bmargin;
    }

    return b_eof() ? 0 : (ptr - buffer);
}

SSIZE ibstream::extractLength()
{
    if (fp) {
        long fptr = ftell(fp);

        fseek(fp, -8L, SEEK_END);
        fread(&file_length, sizeof(file_length), 1, fp);
        fseek(fp, fptr, SEEK_SET);
    }

    return file_length;
}

bool ibstream::b_eof() const
{
    return count > file_length;
}

bool ibstream::f_eof() const
{
    return feof(fp);
}
