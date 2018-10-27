#pragma once
#include "Coder.h"
#include "Tree.h"
#include "settings.h"
#include <vector>

class LZ78Encoder : public Encoder {
    struct Pair {
        size_t prefix_state, state;
    };

public:
    LZ78Encoder(char const* infile, char const* outfile)
        : Encoder(infile, outfile)
    {
    }

    void encode();
};

class LZ78Decoder : public Decoder {

    static constexpr unsigned ordL2N[65] = {
        0, 0, 1, 2, 3, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 58
    };

    struct Code {
        //size_t prefix_state;
        Bit* pattern = nullptr;
        size_t pattern_len = 0;
        //bool children[2] = { false };

        ~Code()
        {
            if (pattern) {
                delete[] pattern;
            }
        }
    };

    SSIZE partition_cnt();

public:
    LZ78Decoder(char const* infile, char const* outfile)
        : Decoder(infile, outfile)
    {
    }

    void decode();
};
