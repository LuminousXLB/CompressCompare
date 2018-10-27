#include "bitstream.h"

size_t obstream::flush()
{
    if (is_open()) {

        int base = 0;
        size_t cnt = pos / 8;

        for (size_t i = 0; i < cnt; i++) {
            buffer[i] = 0;
            for (int j = 0; j < 8; j++) {
                if (bits[base + j]) {
                    buffer[i] |= bit_mask[j];
                }
            }
            base += 8;
        }

        if (cnt * 8 < unsigned(pos)) {
            buffer[cnt] = 0;
            for (int j = cnt * 8; j < pos; j++) {
                if (bits[j]) {
                    buffer[cnt] |= bit_mask[j % 8];
                }
            }
            cnt++;
        }

        cnt = fwrite(buffer, sizeof(Byte), cnt, fp);

        pos = 0;

        D_PFERROR_;

        return cnt;
    } else {
        return 0;
    }
}

void obstream::dumpLength()
{
    flush();
    fwrite(&count, sizeof(count), 1, fp);
}

obstream::obstream()
{
}

obstream::obstream(char const* filename)
    : bstream(filename, false)
{
}

obstream::~obstream()
{
    flush();
}

void obstream::putb(const Bit& b)
{
    if (pos >= BUFFER_SIZE_BIT) {
        flush();
    }

    bits[pos++] = b;
    count++;
}

void obstream::puts(Bit* buffer, size_t cnt)
{
    Bit* ptr = buffer;

    if (pos >= BUFFER_SIZE_BIT) {
        flush();
    }

    size_t bmargin = BUFFER_SIZE_BIT - pos;

    if (cnt > bmargin) {
        memcpy_s(bits + pos, bmargin, ptr, bmargin);
        ptr += bmargin;
        cnt -= bmargin;
        pos += bmargin;
        count += bmargin;
        flush();
    }

    while (cnt > BUFFER_SIZE_BIT) {
        memcpy_s(bits, BUFFER_SIZE_BIT, ptr, BUFFER_SIZE_BIT);
        ptr += BUFFER_SIZE_BIT;
        cnt -= BUFFER_SIZE_BIT;
        pos += BUFFER_SIZE_BIT;
        count += BUFFER_SIZE_BIT;
        flush();
    }

    bmargin = BUFFER_SIZE_BIT - pos;

    if (cnt <= bmargin) {
        memcpy_s(bits + pos, cnt, ptr, cnt);
        ptr += cnt;
        pos += cnt;
        count += cnt;
    }
}
