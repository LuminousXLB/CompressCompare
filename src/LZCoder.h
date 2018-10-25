#pragma once

#include "BitStream.h"

class LZCoder
{
	struct Pair
	{
		size_t prefix_state, state;
	};

	struct Code
	{
		size_t prefix_state;
		bit tail;
		bit* data = NULL;
		size_t data_len = 0;

		~Code() {
			if (data) {
				delete[] data;
			}
		}
	};

	BitStream io;

public:
	LZCoder(const char* infile, const char* outfile);

	void encode();
	void decode();

	void closeFiles();

	~LZCoder();
};

