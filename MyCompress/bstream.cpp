#include "bitstream.h"

bstream::bstream()
    : fp(nullptr)
{
}

bstream::bstream(char const* filename, bool read)
    : fp(nullptr)
{
    open(filename, read);
}

bstream::~bstream()
{
    close();
}

void bstream::open(char const* filename, bool read)
{
    if (fp) {
        fclose(fp);
    }

    if (read) {
        fopen_s(&fp, filename, "rb+");
    } else {
        fopen_s(&fp, filename, "wb+");
    }

    pos = 0;
    count = 0;

    D_FILE_NOT_OPENED;
}

bool bstream::is_open() const
{
    return fp != nullptr;
}

void bstream::close()
{
    if (is_open()) {
        fclose(fp);
        fp = nullptr;
    }
}

FILE* bstream::getFile()
{
    return fp;
}

const SSIZE bstream::getCount() const
{
    return count;
}

int bstream::error() const
{
    return ferror(bstream::fp);
}
