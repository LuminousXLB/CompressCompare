#pragma once

typedef unsigned char bit;
typedef unsigned char byte;

enum Endian
{
	LittleEndian, BigEndian
};

class BitSet
{
private:
	Endian endian;
	byte data;
	char cursor;
public:
	BitSet();
	BitSet(byte data, Endian endian = BigEndian);

	char cursorReset();
	char cursorSet(char pos);
	bit cursorNext();

	const bit get(char idx) const;
	void set(char idx);
	void clear(char idx);

	/*void push(bit data);
	void push(bit* data, byte count);

	void pop(bit data);
	void pop(bit* data, byte count);

	void append(bit data);
	void append(bit* data, byte count);
*/
	const bit operator[](char idx) const;

	void load(bit* &buffer);
	void load(byte data);
	byte dump();

	~BitSet();
};

