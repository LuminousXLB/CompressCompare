#include "File.h"

File::File()
    : fp(nullptr)
    , fn("")
{
}

File::File(const string filename, bool read)
{
    open(filename.c_str(), read);
}

void File::open(const char* filename, bool read)
{
#ifdef LZCoderDBG
    __FUNCTION_ENTRANCE__;
#endif // LZCoderDBG

    fn = filename;

    errno_t eno;

    if (read) {
        eno = fopen_s(&fp, filename, "rb");
    } else {
        eno = fopen_s(&fp, filename, "wb");
    }

#ifdef LZCoderDBG
    __FUNCTION_HERE__;
    cout << fn << " " << hex << fp << dec << endl;
#endif // LZCoderDBG

    if (!fp) {
        throw std::system_error(eno, std::system_category(), "[ERROR] File opening error <" + fn + ">");
    }

#ifdef LZCoderDBG
    __FUNCTION_EXIT__;
#endif // LZCoderDBG
}

bool File::is_open()
{
    return fp;
}

void File::close()
{
#ifdef LZCoderDBG
    __FUNCTION_ENTRANCE__;
    cout << fn << " " << hex << fp << dec << endl;
#endif // LZCoderDBG

    if (fp) {
        fclose(fp);
        fp = nullptr;
    }

#ifdef LZCoderDBG
    __FUNCTION_EXIT__;
#endif // LZCoderDBG
}

errno_t File::checkError()
{
    if (ferror(fp)) {
        throw std::system_error(errno, std::system_category(), "[ERROR] <" + fn + ">");
    }
    return errno;
}

long File::tell()
{
    long int pos = ftell(fp);
    if (ferror(fp)) {
        throw std::system_error(errno, std::system_category(), "[ERROR] ftell() failed in file <" + fn + ">");
    }
    return pos;
}

void File::seek(long offset, int origin)
{
    fseek(fp, offset, origin);
    if (ferror(fp)) {
        throw std::system_error(errno, std::system_category(), "[ERROR] fseek() failed in file <" + fn + ">");
    }
}

File::~File()
{
    close();
}

void inFile::open(const string filename)
{
#ifdef IOBaseDBG
    __FUNCTION_ENTRANCE__;
#endif // IOBaseDBG

    File::open(filename.c_str(), true);

#ifdef IOBaseDBG
    __FUNCTION_HERE__;
#endif // IOBaseDBG

    if (fp) {
        size_t pos = tell();

        seek(0L, SEEK_END);
        file_length = tell();
        seek(pos, SEEK_SET);
    } else {
        file_length = -1;
    }

#ifdef IOBaseDBG
    __FUNCTION_EXIT__;
#endif // IOBaseDBG
}

size_t inFile::read(void* Buffer, size_t BufferSize, size_t ElementSize, size_t ElementCount)
{
    return fread_s(Buffer, BufferSize, ElementSize, ElementCount, fp);
}

size_t inFile::length() const
{
    return file_length;
}

bool inFile::eof() const
{
    return feof(fp);
}

void outFile::open(const string filename)
{
    File::open(filename.c_str(), false);
}

size_t outFile::write(void const* Buffer, size_t ElementSize, size_t ElementCount)
{
    return fwrite(Buffer, ElementSize, ElementCount, fp);
}
