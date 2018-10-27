#include "LZ78Coder.h"
#include "utils.h"
#include <string>

using namespace std;

void prepare(char const* fn, Bit* data, int size)
{
    obstream obs(fn);
    obs.puts(data, size);
    obs.flush();
    obs.close();
}

void encode(char const* in, char const* out)
{
    LZ78Encoder encoder(in, out);
    encoder.encode();
}

void decode(char const* in, char const* out)
{
    LZ78Decoder decoder(in, out);
    decoder.decode();
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
    //naive_display();

    //return 0;

    //FILE* fp;
    //freopen_s(&fp, "E:\\path\\log", "w", stdout);

    string input = "E:\\path\\book.txt";
    string enc = input + ".enc";
    string dec = input + ".dec";

    std::cout << "Hello World!\n";

    encode(input.c_str(), enc.c_str());

    std::cout << "Challenge World!\n";

    try {
        decode(enc.c_str(), dec.c_str());
    } catch (const std::exception&) {
        cout << "exception" << endl;
    }

    std::cout << "GoodBye World!\n";
}
