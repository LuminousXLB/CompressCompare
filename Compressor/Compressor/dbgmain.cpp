#include "settings.h"

#ifndef CMD_TOOL

#include <ctime>
#include <iostream>

#include "index.h"
#include "io/index.h"

#define Encoder ArithEncoder
#define Decoder ArithDecoder

void prepare(const string& fn, const vector<bool>& sample)
{
    oBitBits obb;
    obb.open(fn);
    obb.putUnits(sample);
}

int naive_display();
int more_test(int argc, char const* argv[]);

int main(int argc, char const* argv[])
{
    //FILE* fp;
    //freopen_s(&fp, "E:\\path\\log", "w", stdout);

    more_test(argc, argv);
    //naive_display();
}

int more_test(int argc, char const* argv[])
{
    //string input = "E:\\path\\dbg.txt";
    //string input = "E:\\path\\overtest.txt";
    //string input = "E:\\path\\timetest.txt";
    //string input = "E:\\path\\news.txt";
    string input = "E:\\path\\news.docx";

    if (argc > 1) {
        cout << argv[1] << endl;
        input = argv[1];
    }

    string enc = input + ".enc";
    string dec = input + ".dec";

    std::cout << "Hello World!\n";
    //////////////////////////////////////////////////////////////////////////////
    clock_t enc_t = clock();

    try {
        Encoder encoder(input, enc);
        encoder.encode();
        encoder.close();
    } catch (const std::exception& err) {
        cerr << err.what() << endl;
    }

    enc_t = clock() - enc_t;
    std::cout << "Encode used: " << enc_t << "ms" << endl;
    //////////////////////////////////////////////////////////////////////////////
    clock_t dec_t = clock();

    try {
        Decoder decoder(enc, dec);
        decoder.decode();
        decoder.close();
    } catch (const std::system_error& err) {
        cerr << err.what() << endl;
    }

    dec_t = clock() - dec_t;
    std::cout << "Decode used: " << dec_t << "ms" << endl;
    //////////////////////////////////////////////////////////////////////////////
    std::cout << "GoodBye\n";

    return 0;
}

int naive_display()
{
    std::cout << "Hello World!\n";

    char const* fn0 = "E:\\path\\data.bin";
    char const* fn1 = "E:\\path\\data.enc";
    char const* fn2 = "E:\\path\\data.dec";

    vector<bool> sample1 = { 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 };
    vector<bool> sample2 = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };

    std::cout << "Hello world!\n";

    prepare(fn0, sample1);

    std::cout << "Prepare Finished!\n";

    try {
        Encoder encoder(fn0, fn1);
        encoder.encode();
        encoder.close();
    } catch (const std::exception& err) {
        cerr << err.what() << endl;
    }

    std::cout << "Encode Finished!\n";

    try {
        Decoder decoder(fn1, fn2);
        decoder.decode();
        decoder.close();
    } catch (const std::system_error& err) {
        cerr << err.what() << endl;
    }

    std::cout << "Decode Finished!\n";

    return 0;
}

#endif // !CMD_TOOLS
