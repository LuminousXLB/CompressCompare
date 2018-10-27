#pragma once
#include "bitstream.h"
#include "settings.h"

class Coder {
protected:
    ibstream ibs;
    obstream obs;

public:
    Coder(char const* infile, char const* outfile)
        : ibs(infile)
        , obs(outfile)
    {
    }

    void closeFiles()
    {
        ibs.close();
        obs.close();
    }

    ~Coder()
    {
    }
};

class Encoder : public Coder {
public:
    Encoder(char const* infile, char const* outfile)
        : Coder(infile, outfile)
    {
    }

    virtual void encode() = 0;
};

class Decoder : public Coder {
public:
    Decoder(char const* infile, char const* outfile)
        : Coder(infile, outfile)
    {
    }

    virtual void decode() = 0;
};

template <class T>
unsigned lb(T data)
{
    unsigned cnt = 0;

    while (data > 1) {
        data >>= 1;
        cnt++;
    }

    return cnt;
}
