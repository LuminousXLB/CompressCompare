#include <iostream>
#include <fstream>
#include "LZCoder.h"

using namespace std;

typedef unsigned char byte;

void hexdump(const char * filename) {
	ifstream fin(filename, ios::binary);

	printf("----- hexdump : %s -----", filename);

	char buf;
	size_t cnt = 0;

/*
	fin.seekg(0, fin.beg);
	fin.clear();
	cout << fin.rdstate() << endl;
*/
	while (fin.get(buf)) {
		//cout << " $ " << cnt << endl;
		if (cnt % 32 == 0) {
			printf("\n");
		}
		else if (cnt % 4 == 0) {
			printf(" ");
		}

		printf("%02x", byte(buf));
		cnt++;
	}

	printf("\n------------\n");

	fin.close();
}


void prepare() {
	cout << "--- Write in file ---" << endl;
	ifstream fin("E:\\path\\vacant.bin", ios::binary);
	ofstream fout("E:\\path\\in.bin", ios::binary);

	BitStream io = BitStream(fin, fout);

	bit buffer[] = { 1,0,1,1,0,1,1,1,1,0,1,1,0,1,1,1 };

	for (size_t i = 0; i < 16; i++)
	{
		io.putBit(buffer[i]);
	}
}

void encode() {
	cout << "--- Encode ---" << endl;
	LZCoder encoder = LZCoder("E:\\path\\in.bin", "E:\\path\\out.bin");
	encoder.encode();
}

void decode() {
	cout << "--- Decode ---" << endl;
	LZCoder decoder = LZCoder("E:\\path\\out.bin", "E:\\path\\out.bin.dec");
	decoder.decode();
}

int main()
{
	cout << "Hello World!\n";

	prepare();
	hexdump("E:\\path\\in.bin");

	encode();
	hexdump("E:\\path\\out.bin");

	decode();
	hexdump("E:\\path\\out.bin.dec");
}
//
//void UnitTest_BitSet() {
//	BitSet bs = BitSet(0b10110010);
//
//	bs.cursorReset();
//
//	for (int i = 0; i < 8; i++)
//	{
//		cout << unsigned(bs.cursorNext()) << ' ';
//	}
//
//	cout << endl;
//
//	bs.set(2);
//	bs.set(5);
//
//	bs.clear(3);
//	bs.clear(7);
//
//	bs.cursorReset();
//
//	for (int i = 0; i < 8; i++)
//	{
//		cout << unsigned(bs.cursorNext()) << ' ';
//	}
//
//	cout << endl;
//
//}
//
//void UnitTest_BitStream() {
//	ifstream fin("E:\\path\\in.bin", ios::binary);
//	ofstream fout("E:\\path\\out.bin", ios::binary);
//
//	BitStream io = BitStream(fin, fout);
//
//
//	bit org = 1;
//	bit buf;
//
//	while (io.getBit(buf)) {
//		cout << unsigned(buf) << ' ';
//		io.putBit(org^buf);
//		cout << unsigned(org^buf) << endl;
//		org = buf;
//	}
//
//	for (int i = 0; i < 7; i++)
//	{
//		io.putBit(i % 2);
//	}
//
//	cout << endl;
//
//}
//
