#pragma once

#include "Tree.h"
#include "bitstream.h"
#include <iomanip>
#include <iostream>

void hexdump(Byte* data, size_t count)
{
    for (size_t i = 0; i < count; i) {
        printf("%02x", data[i++]);
        if (i % 32 == 0) {
            printf("\n");
        } else if (i % 4 == 0) {
            printf(" ");
        }
    }
}
