#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "settings.h"

typedef uint64_t SSIZE;
typedef bool Bit;
typedef unsigned char Byte;

using namespace std;

constexpr unsigned BUFFER_SIZE_BYTE = 512;
constexpr unsigned BUFFER_SIZE_BIT = 4096;

const static Byte bit_mask[8] = {
    (Byte)0b10000000,
    (Byte)0b01000000,
    (Byte)0b00100000,
    (Byte)0b00010000,
    (Byte)0b00001000,
    (Byte)0b00000100,
    (Byte)0b00000010,
    (Byte)0b00000001
};

class bstream {
protected:
    FILE* fp;
    Byte buffer[BUFFER_SIZE_BYTE];
    Bit bits[BUFFER_SIZE_BIT];
    SSIZE count;
    size_t pos;

public:
    bstream();
    bstream(char const* filename, bool read);
    ~bstream();

    void open(char const* filename, bool read);
    bool is_open() const;
    void close();

    FILE* getFile();
    const SSIZE getCount() const;

    int error() const;
};

class ibstream : public bstream {
    SSIZE file_length;

    size_t flush();

public:
    ibstream();
    ibstream(char const* filename);
    ~ibstream();

    void open(char const* filename);

    bool getb(Bit& b);
    size_t gets(Bit*& buffer, size_t cnt);

    SSIZE extractLength();

    bool b_eof() const;
    bool f_eof() const;
};

class obstream : public bstream {

public:
    obstream();
    obstream(char const* filename);
    ~obstream();

    size_t flush();

    void dumpLength();

    void putb(const Bit& b);
    void puts(Bit* buffer, size_t cnt);
};
