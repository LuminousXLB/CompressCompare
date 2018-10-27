#pragma once
#include <iostream>

//#define DEBUG

#ifdef DEBUG

#define PRINT_FERROR

#define LZ78_ENCODER_DEBUG_READ
#define LZ78_ENCODER_DEBUG_WRITE
//#define LZ78_ENCODER_DEBUG_CNT

#define LZ78_DECODER_DEBUG

#define DEBUG_HEADER printf("\t[%s : %d] ", __FUNCTION__, __LINE__);

#endif // DEBUG

#ifdef PRINT_FERROR

#define D_PFERROR_             \
    if (ferror(bstream::fp)) { \
        perror("[ERROR] ");    \
    }

#define D_FILE_NOT_OPENED   \
    if (!fp) {              \
        perror("[ERROR] "); \
    }
#else
#define D_PFERROR_
#define D_FILE_NOT_OPENED

#endif // PRINT_FERROR
