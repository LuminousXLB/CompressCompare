#include "LZ78ByteCoder.h"
#include "LZ78Coder.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

template <class T>
void encode(char const* in, char const* out)
{
    T encoder(in, out);
    try {
        encoder.encode();
    } catch (const char* str) {
        cerr << str << endl;
    }
}

template <class T>
void decode(char const* in, char const* out)
{
    T decoder(in, out);
    try {
        decoder.decode();
    } catch (const char* str) {
        cerr << str << endl;
    }
}

//#define LZ78BitCoder

#define LZ78ByteCoder

#ifdef LZ78ByteCoder

int main(int argc, char const* argv[])
{
    std::cout << "Hello World!\n";

    string input = "E:\\path\\log";

    if (argc > 1) {
        cout << argv[1] << endl;
        input = argv[1];
    }

    string enc = input + ".enc";
    string dec = input + ".dec";

    encode<LZ78ByteEncoder>(input.c_str(), enc.c_str());
    decode<LZ78ByteDecoder>(enc.c_str(), dec.c_str());
}

#endif // LZ78ByteCoder

#ifdef LZ78BitCoder

void prepare(char const* fn, Bit* data, int size)
{
    obstream obs(fn);
    obs.puts(data, size);
    obs.flush();
    obs.close();
}

void naive_display()
{
    std::cout << "Hello World!\n";

    char const* fn0 = "E:\\path\\data.bin";
    char const* fn1 = "E:\\path\\data.bin.enc";
    char const* fn2 = "E:\\path\\data.bin.dec";

    //const int size = 24;
    //Bit Sample[size] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };

    const int size = 16;
    Bit Sample[16] = { 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 };

    prepare(fn0, Sample, size);
    encode(fn0, fn1);

    std::cout << "Hello World!\n";

    decode(fn1, fn2);

    std::cout << "GoodBye World!\n";
}

int main(int argc, char const* argv[])
{

    string input = "E:\\path\\a.exe";

    if (argc > 1) {
        cout << argv[1] << endl;
        input = argv[1];
    }

    string enc = input + ".enc";
    string dec = input + ".dec";

    std::cout << "Hello World!\n";

    clock_t enc_t = clock();
    encode<LZ78Encoder>(input.c_str(), enc.c_str());
    enc_t = clock() - enc_t;

    std::cout << "Encode used: " << enc_t << "ms" << endl;

    clock_t dec_t = clock();
    decode<LZ78Decoder>(enc.c_str(), dec.c_str());
    dec_t = clock() - dec_t;
    std::cout << "Decode used: " << enc_t << "ms" << endl;

    std::cout << "GoodBye\n";
}

#endif // LZ78BitCoder
