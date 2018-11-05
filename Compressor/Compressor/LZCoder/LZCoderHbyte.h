#pragma once

#include "LZCoderBase.h"
#include "common.h"

using namespace std;

class LZEncoderHbyte : public LZEncoderBase<uint8_t, 16> {
public:
    LZEncoderHbyte(string infile, string outfile)
    {
        io.open(new iHbyteUnit(), infile, new oBitHbyte(), outfile);
    }
};

class LZDecoderHbyte : public LZDecoderBase<uint8_t, 16> {
    //	static constexpr unsigned ordL2N[65] = {
    //		0, 0, 1, 2, 3, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 58
    //	};
    //
    size_t partition_cnt()
    {
        const size_t file_Len = io.extractLength();

        //		unsigned ord_pC = ordL2N[lb(file_Len)];
        //
        //		size_t part_Cnt = (file_Len + (1ull << (ord_pC + 1)) - 2) / (ord_pC + 2);
        //
        //		if (lb(part_Cnt) == ord_pC) {
        //			return part_Cnt;
        //		}
        //
        //		ord_pC--;
        //		part_Cnt = (file_Len + (1ull << (ord_pC + 1)) - 2) / (ord_pC + 2);
        //
        //		if (lb(part_Cnt) == ord_pC) {
        //			return part_Cnt;
        //		}
        //
        return lb(file_Len);

        throw file_Len;

        return 0;
    }

public:
    LZDecoderHbyte(string infile, string outfile)
    {
        io.open(new iBitHbyte(), infile, new oHbyteUnit(), outfile);
    }
};
