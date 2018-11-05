#include <ctime>
#include <iostream>

#include "index.h"

using namespace std;

template <class Encoder>
void encode(string input, string output)
{
    cout << "Encoding " << input << endl;
    clock_t enc_t = clock();

    try {
        Encoder encoder(input, output);
        cout << " > Initialize Complete" << endl;

        encoder.encode();
        cout << " > Encode Finished" << endl;

        encoder.close();
    } catch (const std::exception& err) {
        cerr << err.what() << endl;
    }

    enc_t = clock() - enc_t;
    cout << "Encode <" << input << "> used: " << (double(enc_t) / CLOCKS_PER_SEC) << "s" << endl;
    cout << "Ouput file is: " << output << endl;
}

template <class Decoder>
void decode(string input, string output)
{
    cout << "Decoding " << input << endl;
    clock_t dec_t = clock();

    try {
        Decoder decoder(input, output);
        cout << " > Initialize Complete" << endl;

        decoder.decode();
        cout << " > Decode Finished" << endl;

        decoder.close();
    } catch (const std::system_error& err) {
        cerr << err.what() << endl;
    }

    dec_t = clock() - dec_t;
    cout << "Decode <" << input << "> used: " << (double(dec_t) / CLOCKS_PER_SEC) << "s" << endl;
    cout << "Ouput file is: " << output << endl;
}

int main(int argc, char const* argv[])
{

    if (argc < 4) {
        cout << "This is a cmd tool" << endl
             << "Usage: " << endl;
        cout << "\tCompressor.exe <action> <coder> <src> [<dst>]" << endl
             << endl
             << "\t<action> - enc : encode" << endl
             << "\t           dec : decode" << endl
             << endl
             << "\t <coder> - lz1: LZCoder, bit as a symbol" << endl
             << "\t         - lz8: LZCoder, byte as a symbol" << endl
             << "\t         - lz4: LZCoder, half byte as a symbol" << endl
             << "\t         - arith: ArithCoder, half byte as a symbol" << endl;
    } else {

        string action = argv[1];
        string coder = argv[2];
        string input = argv[3];
        string output;
        if (argc == 4) {
            output = input + "." + coder + "." + action;
        } else {
            output = argv[4];
        }

        cout << endl;

        if (action == "enc") {
            if (coder == "lz1") {
                encode<LZEncoderBit>(input, output);
            } else if (coder == "lz8") {
                encode<LZEncoderByte>(input, output);
            } else if (coder == "lz4") {
                encode<LZEncoderHbyte>(input, output);
            } else if (coder == "arith") {
                cout << "Warning: ArithEncoder may take really a long time to encode the file" << endl;
                encode<ArithEncoder>(input, output);
            } else {
                cout << "Invalid Encoder" << endl;
                exit(EXIT_FAILURE);
            }
        } else if (action == "dec") {
            if (coder == "lz1") {
                decode<LZDecoderBit>(input, output);
            } else if (coder == "lz8") {
                decode<LZDecoderByte>(input, output);
            } else if (coder == "lz4") {
                decode<LZDecoderHbyte>(input, output);
            } else if (coder == "arith") {
                cout << "Warning: ArithDecoder may take a long time to decode the file" << endl;
                decode<ArithDecoder>(input, output);
            } else {
                cout << "Invalid Encoder" << endl;
                exit(EXIT_FAILURE);
            }
        } else {
            cout << "Invalid action";
            exit(EXIT_FAILURE);
        }

        cout << endl;
    }

    return 0;
}
