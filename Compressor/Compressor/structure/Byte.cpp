#include "Byte.h"

Byte::Byte()
{
    clear();
}

Byte::Byte(const uint8_t& val)
{
    memcpy_s(&b_, 1, &val, 1);
}

Byte::Byte(const array<bool, 8>& val)
{
    b_.x00 = val[0];
    b_.x01 = val[1];
    b_.x02 = val[2];
    b_.x03 = val[3];
    b_.x04 = val[4];
    b_.x05 = val[5];
    b_.x06 = val[6];
    b_.x07 = val[7];
}

bool Byte::get(int pos) const
{
    switch (pos) {
    case 0:
        return b_.x00;
        break;
    case 1:
        return b_.x01;
        break;
    case 2:
        return b_.x02;
        break;
    case 3:
        return b_.x03;
        break;
    case 4:
        return b_.x04;
        break;
    case 5:
        return b_.x05;
        break;
    case 6:
        return b_.x06;
        break;
    case 7:
        return b_.x07;
        break;

    default:
        throw out_of_range(string("[ERROR] Out of range caught at ") + string(__FUNCTION__) + string("with index ") + to_string(pos));
        break;
    }
}

bool Byte::operator[](int pos) const
{
    return get(pos);
}

void Byte::set(int pos, bool val)
{
    switch (pos) {
    case 0:
        b_.x00 = val;
        break;
    case 1:
        b_.x01 = val;
        break;
    case 2:
        b_.x02 = val;
        break;
    case 3:
        b_.x03 = val;
        break;
    case 4:
        b_.x04 = val;
        break;
    case 5:
        b_.x05 = val;
        break;
    case 6:
        b_.x06 = val;
        break;
    case 7:
        b_.x07 = val;
        break;

    default:
        throw out_of_range(string("[ERROR] Out of range caught at ") + string(__FUNCTION__) + string("with index ") + to_string(pos));
        break;
    }
}

void Byte::clr(int pos)
{
    set(pos, 0);
}

void Byte::clear()
{
    memset(&b_, 0, 1);
}

void Byte::flip(int pos)
{
    switch (pos) {
    case 0:
        b_.x00 = !b_.x00;
        break;
    case 1:
        b_.x01 = !b_.x01;
        break;
    case 2:
        b_.x02 = !b_.x02;
        break;
    case 3:
        b_.x03 = !b_.x03;
        break;
    case 4:
        b_.x04 = !b_.x04;
        break;
    case 5:
        b_.x05 = !b_.x05;
        break;
    case 6:
        b_.x06 = !b_.x06;
        break;
    case 7:
        b_.x07 = !b_.x07;
        break;

    default:
        throw out_of_range(string("[ERROR] Out of range caught at ") + string(__FUNCTION__) + string("with index ") + to_string(pos));
        break;
    }
}

uint8_t Byte::to_int()
{
    uint8_t val = 0;
    memcpy_s(&val, 1, &b_, 1);

    return val;
}

string Byte::toString()
{
    return to_string(b_.x00) + to_string(b_.x01) + to_string(b_.x02) + to_string(b_.x03) + to_string(b_.x04) + to_string(b_.x05) + to_string(b_.x06) + to_string(b_.x07);
}
