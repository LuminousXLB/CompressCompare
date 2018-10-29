#include "utils.h"
#include <cstdio>

void hexdump(unsigned char* data, size_t count)
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
