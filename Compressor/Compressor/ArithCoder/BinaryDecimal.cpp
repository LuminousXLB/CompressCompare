#include "BinaryDecimal.h"

short BinaryDecimal::compare(const vector<uint8_t>& opl, const vector<uint8_t>& opr)
{
    size_t minsiz = (opl.size() < opr.size()) ? opl.size() : opr.size();
    for (size_t i = 0; i < minsiz; i++) {
        if (opl[i] > opr[i]) {
            return 1;
        } else if (opl[i] < opr[i]) {
            return -1;
        }
    }

    for (size_t i = minsiz; i < opl.size(); i++) {
        if (opl[i]) {
            return 1;
        }
    }

    for (size_t i = minsiz; i < opr.size(); i++) {
        if (opr[i]) {
            return -1;
        }
    }

    return 0;
}

BinaryDecimal::BinaryDecimal()
{
    data.push_back(0);
}

BinaryDecimal::BinaryDecimal(int idx)
{
    idx--;
    data.resize(idx / 8, 0);
    data.push_back(uint8_t(0x80) >> (idx % 8));
}

BinaryDecimal::BinaryDecimal(const BinaryDecimal& ano)
    : data(ano.data)
{
}

BinaryDecimal BinaryDecimal::add(const BinaryDecimal& accu, int prob)
{
    if (prob == 0) {
        return accu;
    } else {
        return BinaryDecimal(accu).add(prob);
    }
}

BinaryDecimal& BinaryDecimal::add(uint64_t prob)
{
    size_t block = size_t((prob - 1) / 8);

    if (data.size() <= block) {
        data.resize(block + 1, 0);
    }

    uint8_t carry = uint8_t(0x80) >> (prob - 1) % 8;

    for (long i = block; i >= 0; i--) {
        int sum = data[i] + carry;
        data[i] = sum & 0xff;
        carry = sum >> 8;
    }

    assert(carry == 0);

    return *this;
}

BinaryDecimal& BinaryDecimal::shiftAdd(const BinaryDecimal& accu, uint64_t range)
{
    size_t newsize = size_t((range + 7) / 8 + accu.data.size());
    if (data.size() < newsize) {
        data.resize(newsize, 0);
    }

    size_t block = size_t(range / 8);

    uint8_t bit_off = range % 8;
    uint8_t bit_cmp = 8 - bit_off;

    size_t blk_off = block + (bit_off != 0);

    uint8_t carry = 0;
    for (long i = accu.data.size() - 1; i >= 0; i--) {
        int sum = data[i + blk_off] + ((accu.data[i] << bit_cmp) & 0xff) + carry;
        data[i + blk_off] = sum & 0xff;
        carry = (accu.data[i] >> bit_off) + (sum >> 8);
    }

    for (long i = block; carry != 0 && i >= 0; i--) {
        int sum = data[i] + carry;
        data[i] = sum & 0xff;
        carry = sum >> 8;
    }

    assert(carry == 0);

    return *this;
}

uint8_t BinaryDecimal::unwrap(const BinaryDecimal& accu, int prob, uint8_t margin)
{

    size_t block = prob / 8;
    uint8_t bit_off = prob % 8;
    uint8_t bit_cmp = 8 - bit_off;

    size_t blk_off = block + (bit_off != 0);

    uint8_t borrow = 0;
    for (long i = accu.data.size() - 1; i >= 0; i--) {
        int diff = data[i] - accu.data[i] - borrow;
        borrow = diff < 0;
        data[i] = diff < 0 ? diff + 0xff : diff;
    }

    assert(borrow == 0);

    for (size_t i = 0; i < block; i++) {
        assert(data[0] == 0);
        data.erase(data.begin());
    }

    assert((data[0] >> bit_cmp) == 0);

    data[0] = (data[0] << bit_off) | (data[1] >> bit_cmp);
    if (data.size() > 2) {
        data[1] = (data[1] << bit_off) | (data[2] >> bit_cmp);

        if (bit_off + margin == 8) {
            margin = 0;
            data.erase(data.begin() + 2);
        } else if (bit_off + margin < 8) {
            data[2] <<= bit_off;
            margin += bit_off;
        } else if (bit_off + margin > 8) {
            data.erase(data.begin() + 2);
            margin -= 8 - bit_off;
            data[1] |= data[2] >> (8 - margin);
            data[2] <<= margin;
        }
    } else {
        data[1] = (data[1] << bit_off);
    }
    return margin;
}

size_t BinaryDecimal::size() const
{
    return data.size();
}

size_t BinaryDecimal::shrink()
{
    while (data.back() == 0) {
        data.pop_back();
    }
    data.shrink_to_fit();
    return data.size();
}

void BinaryDecimal::resize(const size_t newsize)
{
    data.resize(newsize, 0);
}

uint8_t* BinaryDecimal::data_ptr()
{
    return data.data();
}

bool operator>(const BinaryDecimal& opl, const BinaryDecimal& opr)
{
    return BinaryDecimal::compare(opl.data, opr.data) > 0;
}

bool operator==(const BinaryDecimal& opl, const BinaryDecimal& opr)
{
    return BinaryDecimal::compare(opl.data, opr.data) == 0;
}

bool operator<(const BinaryDecimal& opl, const BinaryDecimal& opr)
{
    return BinaryDecimal::compare(opl.data, opr.data) < 0;
}
