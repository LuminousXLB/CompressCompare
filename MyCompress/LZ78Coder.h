#pragma once

#include "Tree.h"
#include "bitstream.h"
#include "settings.h"
#include "utils.h"
#include <vector>

class LZ78Coder {
protected:
    ibstream ibs;
    obstream obs;

public:
    LZ78Coder(char const* infile, char const* outfile)
        : ibs(infile)
        , obs(outfile)
    {
        if (!ibs.is_open() || !obs.is_open()) {
            throw "File not Opened";
        }
    }

    void closeFiles()
    {
        ibs.close();
        obs.close();
    }

    ~LZ78Coder()
    {
        closeFiles();
    }
};

class LZ78Encoder : public LZ78Coder {
    struct Pair {
        size_t prefix_state, state;
    };

public:
    LZ78Encoder(char const* infile, char const* outfile)
        : LZ78Coder(infile, outfile)
    {
    }

    void encode();
};

class LZ78Decoder : public LZ78Coder {

    static constexpr unsigned ordL2N[65] = {
        0, 0, 1, 2, 3, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 58
    };

    struct Code {
        Bit* pattern = nullptr;
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
    LZ78Decoder(char const* infile, char const* outfile)
        : LZ78Coder(infile, outfile)
    {
    }

    void decode();
};
