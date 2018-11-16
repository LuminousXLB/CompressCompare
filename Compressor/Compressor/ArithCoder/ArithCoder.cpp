#include "ArithCoder.h"

array<uint8_t, 16> ArithEncoder::stat_unify()
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
        if (arr[i]) {
            htree.push(i, arr[i] / cnt);
        }
    }

    htNode* root = htree.build();
    assert(!root->isLeaf());

    array<uint8_t, 16> probrec = { 0 };
    traverse(root, 1, probrec);

    return probrec;
}

void ArithEncoder::traverse(ArithEncoder::htNode* node, uint8_t prob, array<uint8_t, 16>& rec)
{
    if ((*node)[0]->isLeaf()) {
        rec[(*node)[0]->getVal()] = prob;
    } else {
        traverse((*node)[0], prob + 1, rec);
    }

    if ((*node)[1]->isLeaf()) {
        rec[(*node)[1]->getVal()] = prob;
    } else {
        traverse((*node)[1], prob + 1, rec);
    }
}

ArithEncoder::ArithEncoder(string infile, string outfile)
{
    ifile.open(infile);
    ofile.open(outfile);

    ifile.seek(0, SEEK_END);
    size_t fleng = ifile.tell();
    ifile.seek(0, SEEK_SET);

    uint64_t len = fleng * 2;
    ofile.write(&len, sizeof(uint64_t), 1);

    buffer.resize(fleng);
    ifile.read(&buffer[0], fleng, 1, fleng);

    ifile.close();
}

void ArithEncoder::encode()
{
    // dump the probs of the symbols
    array<uint8_t, 16> probs = stat_unify();
    ofile.write(probs.data(), sizeof(uint8_t), 16);

    // calculate the accumulate probs
    array<BinaryDecimal, 16> accup;
    for (size_t i = 1; i < 16; i++) {
        accup[i] = BinaryDecimal::add(accup[i - 1], probs[i - 1]);
    }

    // encode
    uint8_t u1 = (buffer[0] >> 4) & 0x0f;

    BinaryDecimal base = accup[u1];
    uint64_t range = probs[u1];

    uint8_t u2 = (buffer[0] & 0x0f);

    base.shiftAdd(accup[u2], range);
    range += probs[u2];

    size_t fleng = buffer.size();
    for (size_t i = 1; i < fleng; i++) {
        u1 = (buffer[i] >> 4) & 0x0f;

        base.shiftAdd(accup[u1], range);
        range += probs[u1];

        u2 = (buffer[i] & 0x0f);

        base.shiftAdd(accup[u2], range);
        range += probs[u2];
    }

    // calculate the code length and dump the result
    uint64_t L = range / 8 + (range % 8 != 0);

    if (L <= base.size()) {
        base.add(L * 8);
    }

    ofile.write(base.data_ptr(), sizeof(uint8_t), base.size());
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
    ifile.read(&fileleng, sizeof(uint64_t), sizeof(uint64_t), 1);

    // load the probs of the symbols
    ifile.read(probs.data(), probs.size(), sizeof(uint8_t), probs.size());

    code.resize(size_t(fileleng >> 1));
    ifile.read(code.data_ptr(), code.size(), sizeof(uint8_t), code.size());

    ifile.close();
    ofile.open(outfile);
}

void ArithDecoder::decode()
{
    // do the decoding

    // calculate the accumulate probs
    array<BinaryDecimal, 16> accup;
    for (size_t i = 1; i < 16; i++) {
        accup[i] = BinaryDecimal::add(accup[i - 1], probs[i - 1]);
    }

    uint8_t margin = 0;

    for (size_t cnt = 0; cnt < fileleng; cnt++) {
        uint8_t symbol = 255;

        if (code < accup[6]) {
            symbol = 0x5;
            for (uint8_t i = 1; i < 6; i++) {
                if (code < accup[i]) {
                    symbol = i - 1;
                    break;
                }
            }
        } else if (code < accup[7]) {
            symbol = 0x6;
        } else if (code < accup[8]) {
            symbol = 0x7;
        } else {
            symbol = 0xf;
            for (uint8_t i = 9; i < 16; i++) {
                if (code < accup[i]) {
                    symbol = i - 1;
                    break;
                }
            }
        }

        if (symbol == 255) {
            symbol = 0xf;
        }

        ofile.putUnit(symbol);
        margin = code.unwrap(accup[symbol], probs[symbol], margin);
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
