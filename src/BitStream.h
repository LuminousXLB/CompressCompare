#pragma once

#include <fstream>
#include <ctime>

#include "BitSet.h"

using namespace std;

class BitStream
{
	// Big-Endian
	ifstream *is;
	ofstream *os;

	BitSet ibuf, obuf;
	byte ipos, opos;
	uint64_t icnt, ocnt;

public:
	clock_t timer;

	BitStream(ifstream &is, ofstream &os);
	BitStream(const char * infile, const char * outfile);

	ifstream& getBit(bit &buffer);
	void putBit(bit data);

	uint64_t icount() const;
	uint64_t ocount() const;

	ifstream& getIs();
	ofstream& getOs();

	void flush();

	bool eof();

	ios::iostate rdstate() const;

	void closeFiles();

	~BitStream();
};

