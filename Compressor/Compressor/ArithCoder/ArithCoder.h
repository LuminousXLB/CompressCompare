#pragma once

#include "BinaryDecimal.h"
#include "common.h"

class ArithEncoder {
    using htNode = HuffmanNodeBase<uint8_t, double>;

    inFile ifile;
    outFile ofile;

    HuffmanTree<uint8_t, double> htree;
    vector<uint8_t> buffer;

    array<uint8_t, 16> stat_unify();
    void traverse(htNode* node, uint8_t prob, array<uint8_t, 16>& rec);

public:
    ArithEncoder(string infile, string outfile);

    void encode();

    void close();
    ~ArithEncoder();
};

class ArithDecoder {
    inFile ifile;
    oHbyteUnit ofile;

    uint64_t fileleng;
    uint64_t count = 0;
    array<uint8_t, 16> probs;

    BinaryDecimal code;

public:
    ArithDecoder(string infile, string outfile);

    void decode();

    void close();
    ~ArithDecoder();
};
