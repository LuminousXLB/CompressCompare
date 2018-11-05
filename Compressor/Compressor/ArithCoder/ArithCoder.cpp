#include "ArithCoder.h"

array<BinaryDecimal, 16> ArithEncoder::stat_unify()
{
    array<size_t, 16> arr = { 0 };
    double cnt = 0;
    for (size_t i = 0; i < buffer.size(); i++) {
        uint8_t val = buffer[i];
        arr[val & 0x0f]++;
        arr[(val >> 4) & 0x0f]++;
        cnt += 2;
    }

    for (uint8_t i = 0; i < 16; i++) {
        htree.push(i, arr[i] / cnt);
    }

    htNode* root = htree.build();

    array<BinaryDecimal, 16> probrec;

    assert(!root->isLeaf());

    vector<bool> init;
    init.resize(1);
    init[0] = true;

    if ((*root)[0]->isLeaf()) {
        probrec[(*root)[0]->getVal()] = BinaryDecimal(init);
    } else {
        traverse((*root)[0], BinaryDecimal(init) >> 1, probrec);
    }

    if ((*root)[1]->isLeaf()) {
        probrec[(*root)[1]->getVal()] = BinaryDecimal(init);
    } else {
        traverse((*root)[1], BinaryDecimal(init) >> 1, probrec);
    }

#ifdef ArithEncoderProbDBG
    for (size_t i = 0; i < 16; i++) {
        __DBGHeader__;
        for (size_t j = 0; j < 16 && j < probrec[i].size(); j++) {
            printf("%d", probrec[i][j]);
        }
        cout << endl;
    }
#endif // ArithEncoderProbDBG

    return probrec;
}

void ArithEncoder::traverse(ArithEncoder::htNode* node, BinaryDecimal prob, array<BinaryDecimal, 16>& rec)
{
    if ((*node)[0]->isLeaf()) {
        rec[(*node)[0]->getVal()] = prob;
    } else {
        traverse((*node)[0], prob >> 1, rec);
    }

    if ((*node)[1]->isLeaf()) {
        rec[(*node)[1]->getVal()] = prob;
    } else {
        traverse((*node)[1], prob >> 1, rec);
    }
}

ArithEncoder::ArithEncoder(string infile, string outfile)
{
    ifile.open(infile);
    ofile.open(outfile);

    ifile.seek(0, SEEK_END);
    size_t fleng = ifile.tell();
    ifile.seek(0, SEEK_SET);

    ofile.putBits<uint64_t>(fleng * 2, 64);

    buffer.resize(fleng);
    ifile.read(&buffer[0], fleng, 1, fleng);

    ifile.close();
}

void ArithEncoder::encode()
{
    // dump the probs of the symbols
    array<BinaryDecimal, 16> probs = stat_unify();
    for (size_t i = 0; i < 16; i++) {
        probs[i].dump(ofile, 16);
    }

    // calculate the accumulate probs
    array<BinaryDecimal, 16> accup;
    accup[0] = BinaryDecimal(1, false);
    for (size_t i = 1; i < 16; i++) {
        accup[i] = probs[i - 1] + accup[i - 1];
    }

#ifdef ArithEncoderProbDBG
    for (size_t i = 0; i < 16; i++) {
        __DBGHeader__;
        for (size_t j = 0; j < 16 && j < accup[i].size(); j++) {
            printf("%d", accup[i][j]);
        }
        cout << endl;
    }
#endif // ArithEncoderProbDBG

    // encode
    uint8_t u1 = (buffer[0] >> 4) & 0x0f;

    BinaryDecimal base = accup[u1];
    BinaryDecimal range = probs[u1];

    base.reserve(buffer.size());
    range.reserve(16);
#ifdef ArithEncoderCodeDBG
    __DBGHeader__;
    printf("%x | - | ", u1);
    for (size_t i = 0; i < 16 && i < base.size(); i++) {
        printf("%d", base[i]);
    }

    printf(" | ");

    for (size_t i = 0; i < 16 && i < range.size(); i++) {
        printf("%d", range[i]);
    }

    cout << endl;
#endif // ArithEncoderCodeDBG

    uint8_t u2 = (buffer[0] & 0x0f);

    base += accup[u2] * range;
    range *= probs[u2];

#ifdef ArithEncoderCodeDBG
    __DBGHeader__;
    printf("%x | - | ", u2);
    for (size_t i = 0; i < 16 && i < base.size(); i++) {
        printf("%d", base[i]);
    }

    printf(" | ");

    for (size_t i = 0; i < 16 && i < range.size(); i++) {
        printf("%d", range[i]);
    }

    cout << endl;
#endif // ArithEncoderCodeDBG

    size_t fleng = buffer.size();
    for (size_t i = 1; i < fleng; i++) {
        u1 = (buffer[i] >> 4) & 0x0f;

        base += accup[u1] * range;
        range *= probs[u1];

#ifdef ArithEncoderCodeDBG
        __DBGHeader__;
        printf("%x | - | ", u1);
        for (size_t i = 0; i < 16 && i < base.size(); i++) {
            printf("%d", base[i]);
        }

        printf(" | ");

        for (size_t i = 0; i < 16 && i < range.size(); i++) {
            printf("%d", range[i]);
        }

        cout << endl;
#endif // ArithEncoderCodeDBG

        u2 = (buffer[i] & 0x0f);

        base += accup[u2] * range;
        range *= probs[u2];

#ifdef ArithEncoderCodeDBG
        __DBGHeader__;
        printf("%x | - | ", u2);
        for (size_t i = 0; i < 16 && i < base.size(); i++) {
            printf("%d", base[i]);
        }

        printf(" | ");

        for (size_t i = 0; i < 16 && i < range.size(); i++) {
            printf("%d", range[i]);
        }

        cout << endl;
#endif // ArithEncoderCodeDBG
    }

    // calculate the code length and dump the result
    size_t L = range.nextSet(0);
    size_t afterL = base.nextSet(L + 1);
    if (afterL != base.size()) {
        base += range;
    }

    base.dump(ofile, L + 1);
}

void ArithEncoder::close()
{
    ofile.close();
}

ArithEncoder::~ArithEncoder()
{
    close();
}

ArithDecoder::ArithDecoder(string infile, string outfile)
{
    ifile.open(infile);

    // load the file length
    ifile.getBits(fileleng, 64);

    // load the probs of the symbols
    for (size_t i = 0; i < 16; i++) {
        probs[i].load(ifile, 16);
    }

    bool buf;
    vector<bool> codebuf;
    codebuf.reserve(size_t(fileleng * 4));

#ifdef ArithDecoderCodeDBG
    __DBGHeader__;
#endif // ArithDecoderCodeDBG

    while (ifile.getBit(buf)) {
        codebuf.push_back(buf);
#ifdef ArithDecoderCodeDBG
        printf("%d", buf);
#endif // ArithDecoderCodeDBG
    }
#ifdef ArithDecoderCodeDBG
    cout << endl;
#endif // ArithDecoderCodeDBG

    code = BinaryDecimal(codebuf);

#ifdef ArithDecoderProbDBG
    for (size_t i = 0; i < 16; i++) {
        __DBGHeader__;
        for (size_t j = 0; j < 16; j++) {
            printf("%d", probs[i][j]);
        }
        cout << endl;
    }
#endif // ArithDecoderProbDBG

    ifile.close();
    ofile.open(outfile);
}

void ArithDecoder::decode()
{
    // do the decoding
    array<BinaryDecimal, 16> accup;
    accup[0] = BinaryDecimal(1, false);
    for (size_t i = 1; i < 16; i++) {
        accup[i] = probs[i - 1] + accup[i - 1];
    }

#ifdef ArithDecoderProbDBG
    for (size_t i = 0; i < 16; i++) {
        __DBGHeader__;
        for (size_t j = 0; j < 16; j++) {
            printf("%d", accup[i][j]);
        }
        cout << endl;
    }
#endif // ArithDecoderProbDBG

    for (size_t cnt = 0; cnt < fileleng; cnt++) {
        uint8_t symbol = 255;
        for (uint8_t i = 1; i < 16; i++) {
            if (code < accup[i]) {
                symbol = i - 1;
                break;
            }
        }

        if (symbol == 255) {
            symbol = 0xf;
        }

        ofile.putUnit(symbol);
#ifdef ArithDecoderOutDBG
        printf("%x", symbol);
        fflush(stdout);
#endif // ArithDecoderOutDBG

        code -= accup[symbol];
#ifdef ArithDecoderDuraDBG
        __DBGHeader__;
        printf("%x | - | ", symbol);
        for (size_t i = 0; i < 16 && i < code.size(); i++) {
            printf("%d", code[i]);
        }
        cout << endl;
#endif // ArithDecoderDuraDBG

        code /= probs[symbol];
#ifdef ArithDecoderDuraDBG
        __DBGHeader__;
        printf("%x | / | ", symbol);
        for (size_t i = 0; i < 16 && i < code.size(); i++) {
            printf("%d", code[i]);
        }
        cout << endl;
#endif // ArithDecoderDuraDBG
    }

    ofile.flush();
}

void ArithDecoder::close()
{
    ofile.close();
}

ArithDecoder::~ArithDecoder()
{
    close();
}
