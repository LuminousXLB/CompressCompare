#include "BitStream.h"


BitStream::BitStream(ifstream & ist, ofstream & ost) : is(&ist), os(&ost), ipos(8), opos(0), icnt(0), ocnt(0), timer(0)
{
}

BitStream::BitStream(const char * infile, const char * outfile) : ipos(8), opos(0), icnt(0), ocnt(0), timer(0)
{
	is = new ifstream(infile, ios::binary);
	os = new ofstream(outfile, ios::binary);
}

ifstream & BitStream::getBit(bit & buffer)
{
	if (ipos < 0 || ipos >7) {
		ipos = 0;
		ibuf = BitSet(is->get());
	}

	buffer = ibuf[ipos++];

	icnt++;
	return (*is);
}

void BitStream::putBit(bit data)
{
	clock_t init = clock();

	if (opos < 0 || opos >7) {
		opos = 0;
		os->put(obuf.dump());
		obuf = BitSet(0x00);
	}

	if (data) {
		obuf.set(opos++);
	}
	else {
		obuf.clear(opos++);
	}

	ocnt++;

	timer += clock() - init;
}

uint64_t BitStream::icount() const
{
	return icnt;
}

uint64_t BitStream::ocount() const
{
	return ocnt;
}

ifstream & BitStream::getIs()
{
	return *is;
}

ofstream & BitStream::getOs()
{
	return *os;
}

void BitStream::flush()
{
	opos = 0;
	os->put(obuf.dump());
	obuf = BitSet(0x00);
}

bool BitStream::eof()
{
	return is->eof();
}

ios::iostate BitStream::rdstate() const
{
	return is->rdstate();
}

void BitStream::closeFiles()
{
	is->close();
	os->close();
}

BitStream::~BitStream()
{
	flush();
	is->close();
	os->close();
}
