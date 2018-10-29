#pragma once

template <class T>
unsigned lb(T data)
{
    unsigned cnt = 0;

    while (data > 1) {
        data >>= 1;
        cnt++;
    }

    return cnt;
}

void hexdump(unsigned char* data, size_t count);
