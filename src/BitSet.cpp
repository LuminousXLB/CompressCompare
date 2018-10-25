#include "BitSet.h"


BitSet::BitSet() : endian(BigEndian)
{
}

BitSet::BitSet(byte data, Endian endian) : data(data), endian(endian)
{
}

char BitSet::cursorReset()
{
	cursor = 0;
	return cursor;
}

char BitSet::cursorSet(char pos)
{
	cursor = pos;
	return 0;
}

bit BitSet::cursorNext()
{
	byte pos = cursor++;
	return (*this)[pos];
}

const bit BitSet::get(char idx) const
{
	if (idx > 7 || idx < 0) {
		return idx;
	}
	else {
		if (endian == BigEndian) {
			idx = 7 - idx;
		}
		return (data & (0x01 << idx)) != 0;
	}
}

void BitSet::set(char idx)
{
	if (endian == BigEndian) {
		data |= (0x01 << (7 - idx));
	}
	else {
		data |= (0x01 << idx);
	}
}

void BitSet::clear(char idx)
{
	static byte mask[8] = {
		0b11111110,
		0b11111101,
		0b11111011,
		0b11110111,
		0b11101111,
		0b11011111,
		0b10111111,
		0b01111111,
	};

	if (endian == BigEndian) {
		data &= mask[(7 - idx)];
	}
	else {
		data &= mask[idx];
	}
}

const bit BitSet::operator[](char idx) const
{
	return this->get(idx);
}

void BitSet::load(bit *& buffer)
{
	data = 0;
	if (endian == BigEndian) {
		for (int i = 0; i < 8; i++)
		{
			data |= (0x01 << (7 - i));
		}
	}
	else {
		for (int i = 0; i < 8; i++)
		{
			data |= (0x01 << i);
		}
	}
}

void BitSet::load(byte data)
{
	this->data = data;
}

byte BitSet::dump()
{
	return data;
}

BitSet::~BitSet()
{
}
