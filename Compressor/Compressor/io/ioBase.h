#pragma once

#include "File.h"
#include "common.h"

class ioBase {
protected:
    File f;

public:
    bool is_open()
    {
        return f.is_open();
    }

    void close()
    {
        f.close();
    }

    ~ioBase()
    {
        close();
    }
};

///////////////////////////////////////////////////////////////////////////////

class iBase : public ioBase {
protected:
    inFile f;

public:
    void open(string filename)
    {
        f.open(filename);
    }

    virtual bool eof() const
    {
        return f.eof();
    }
};

///////////////////////////////////////////////////////////////////////////////

class oBase : public ioBase {
protected:
    outFile f;

public:
    void open(string filename)
    {
        f.open(filename);
    }
};

///////////////////////////////////////////////////////////////////////////////
