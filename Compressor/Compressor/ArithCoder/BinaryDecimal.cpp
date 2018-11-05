#include "BinaryDecimal.h"

void BinaryDecimal::add(const vector<bool>& ano, size_t off)
{
    if (data.size() < ano.size() + off) {
        data.resize(ano.size() + off, 0);
    }

    bool carry = 0;
    short sum = 0;

    for (size_t i = ano.size() + off - 1; i > off; i--) {
        sum = data[i] + ano[i - off] + carry;
        data[i] = sum % 2;
        carry = sum / 2;
    }

    sum = data[off] + ano[0] + carry;
    data[off] = sum % 2;
    if (off > 0) {
        data[off - 1] = sum / 2;
    }
#ifdef DEBUG
    else {
        assert(sum / 2 == 0);
    }
#endif // DEBUG
}

short BinaryDecimal::compare(const vector<bool>& opl, const vector<bool> opr)
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
    data.reserve(8);
    data.push_back(0);
}

BinaryDecimal::BinaryDecimal(const BinaryDecimal& ano)
    : data(ano.data)
{
}

BinaryDecimal::BinaryDecimal(vector<bool> value)
    : data(value)
{
}

BinaryDecimal::BinaryDecimal(size_t _count, bool _val)
    : data(_count, _val)
{
}

BinaryDecimal::BinaryDecimal(double value, size_t bits)
    : data(bits)
{
#ifdef DEBUG
    assert(value < 1);
#endif // DEBUG

    for (size_t i = 0; i < bits; i++) {
        value *= 2;
        data[i] = int(value);
    }
}

size_t BinaryDecimal::size() const
{
    return data.size();
}

size_t BinaryDecimal::nextSet(size_t start) const
{
    size_t msize = data.size();

    for (size_t i = start; i < msize; i++) {
        if (data[i]) {
            return i;
        }
    }

    return data.size();
}

bool BinaryDecimal::operator[](size_t idx) const
{
    return data[idx];
}

void BinaryDecimal::add(const BinaryDecimal& ano)
{
    if (data.size() < ano.data.size()) {
        data.resize(ano.data.size(), 0);
    }

    bool carry = 0;
    short sum = 0;
    for (size_t i = ano.data.size() - 1; i > 0; i--) {
        sum = data[i] + ano.data[i] + carry;
        data[i] = sum % 2;
        carry = sum / 2;
    }

    sum = data[0] + ano.data[0] + carry;
#ifdef DEBUG
    assert(sum / 2 == 0);
#endif // DEBUG

    data[0] = sum % 2;
}

void BinaryDecimal::mul(const BinaryDecimal& ano)
{
    vector<bool> vec = data;
    data.reserve(vec.size() + ano.size());
    data.assign(vec.size(), 0);

    size_t anos = ano.data.size();
    for (size_t i = 0; i < anos; i++) {
        if (ano.data[i]) {
            add(vec, i + 1);
        }
    }
}

void BinaryDecimal::sub(const BinaryDecimal& ano)
{
#ifdef DEBUG
    assert(compare(data, ano.data) >= 0);
#endif // DEBUG

    if (data.size() < ano.data.size()) {
        data.resize(ano.data.size(), 0);
    }

    bool borrow = 0;
    short diff = 0;
    for (size_t i = ano.data.size() - 1; i > 0; i--) {
        diff = data[i] - borrow - ano.data[i];
        borrow = (diff < 0);
        data[i] = borrow ? (diff + 2) : diff;
    }

    diff = data[0] - borrow - ano.data[0];
#ifdef DEBUG
    assert(diff >= 0);
#endif // DEBUG
    data[0] = diff;
}

void BinaryDecimal::div(const BinaryDecimal& ano)
{
#ifdef DEBUG
    size_t cnt = 0;
    size_t msize = ano.data.size();
    for (size_t i = 0; i < msize; i++) {
        if (ano.data[i]) {
            cnt++;
        }
    }

    assert(cnt == 1);
#endif // DEBUG

    size_t off = 0;
    while (!ano.data[off++]) {
    }

#ifdef DEBUG
    for (size_t i = 0; i < off; i++) {
        assert(data[i] == 0);
    }
#endif // DEBUG

    size_t newlength = data.size() - off;

    for (size_t i = 0; i < newlength; i++) {
        data[i] = data[i + off];
    }

    data.resize(newlength);
}

BinaryDecimal& BinaryDecimal::operator=(const BinaryDecimal& ano)
{
    data.assign(ano.data.begin(), ano.data.end());
    return *this;
}

BinaryDecimal& BinaryDecimal::operator+=(const BinaryDecimal& ano)
{
    add(ano);
    return *this;
}

BinaryDecimal& BinaryDecimal::operator*=(const BinaryDecimal& ano)
{
    mul(ano);
    return *this;
}

BinaryDecimal& BinaryDecimal::operator-=(const BinaryDecimal& ano)
{
    sub(ano);
    return *this;
}

BinaryDecimal& BinaryDecimal::operator/=(const BinaryDecimal& ano)
{
    div(ano);
    return *this;
}

BinaryDecimal& BinaryDecimal::operator>>=(int off)
{
    size_t lastSet = 0;
    size_t siz = data.size();

    for (size_t i = 0; i < siz; i++) {
        if (data[i]) {
            lastSet = i;
        }
    }

    data.resize(lastSet + off + 1);

    for (size_t i = lastSet; i > 0; i--) {
        data[i + off] = data[i];
    }

    data[off] = data[0];

    for (int i = 0; i < off; i++) {
        data[i] = 0;
    }

    return *this;
}

void BinaryDecimal::dump(oBit& obb, size_t len)
{
    for (size_t i = 0; i < len && i < data.size(); i++) {
        obb.putBit(data[i]);
    }

    for (size_t i = data.size(); i < len; i++) {
        obb.putBit(0);
    }

    obb.flush();
}

void BinaryDecimal::load(iBit& ibb, size_t len)
{
    bool buf;
    data.resize(len);
    for (size_t i = 0; i < len; i++) {
        ibb.getBit(buf);
        data[i] = buf;
    }
}

void BinaryDecimal::reserve(size_t size)
{
    data.reserve(size);
}

BinaryDecimal BinaryDecimal::operator>>(int off)
{
    BinaryDecimal res(data.size() + off, 0);

    res.data[0] = 0;
    for (size_t i = 0; i < data.size(); i++) {
        res.data[i + 1] = data[i];
    }

    return res;
}

BinaryDecimal operator+(const BinaryDecimal& opl, const BinaryDecimal& opr)
{
    BinaryDecimal res = opl;
    res.add(opr);
    return res;
}

BinaryDecimal operator*(const BinaryDecimal& opl, const BinaryDecimal& opr)
{
    BinaryDecimal res = opl;
    res.mul(opr);
    return res;
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
