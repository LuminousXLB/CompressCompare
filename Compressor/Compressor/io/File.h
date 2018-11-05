#pragma once

#include "common.h"

using namespace std;

class File {
protected:
    FILE* fp;
    string fn;

public:
    File();
    File(const string filename, bool read);

    void open(const char* filename, bool read);
    bool is_open();
    void close();

    errno_t checkError();

    long tell();
    void seek(long offset, int origin);

    ~File();
};

class inFile : public File {
protected:
    size_t file_length;

public:
    void open(const string filename);

    size_t read(void* Buffer, size_t BufferSize, size_t ElementSize, size_t ElementCount);

    size_t length() const;

    bool eof() const;
};

class outFile : public File {
public:
    void open(const string filename);

    size_t write(void const* Buffer, size_t ElementSize, size_t ElementCount);
};
