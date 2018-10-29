#pragma once

#include "Tree.h"
#include "bitstream.h"
#include "settings.h"
#include "utils.h"
#include <vector>

class LZ78ByteCoder {
};

class LZ78ByteEncoder : public LZ78ByteCoder {
    FILE* ifp;
    obstream obs;

    struct Pair {
        size_t prefix_state, state;
    };

public:
    LZ78ByteEncoder(const char* input, const char* output)
        : obs(output)
    {
        fopen_s(&ifp, input, "rb");
        if (!ifp || !obs.is_open()) {
            throw "File not opened";
        }
#ifdef PRINT_FERROR
        if (ferror(ifp)) {
            perror("[ERROR] ");
        }
#endif // DEBUG
    }

    ~LZ78ByteEncoder()
    {
        fclose(ifp);
        obs.close();
    }

    void encode();
};

class LZ78ByteDecoder : public LZ78ByteCoder {
    ibstream ibs;
    FILE* ofp;

    struct Code {
        Byte* pattern = nullptr;
        size_t pattern_len = 0;

        ~Code()
        {
            if (pattern) {
                delete[] pattern;
            }
        }
    };

    SSIZE partition_cnt();

public:
    LZ78ByteDecoder(const char* input, const char* output)
        : ibs(input)
    {
        fopen_s(&ofp, output, "wb");
        if (!ofp || !ibs.is_open()) {
            throw "File not opened";
        }
#ifdef PRINT_FERROR
        if (ferror(ofp)) {
            perror("[ERROR] ");
        }
#endif // DEBUG
    }

    ~LZ78ByteDecoder()
    {
        ibs.close();
        fclose(ofp);
    }

    void decode();
};
