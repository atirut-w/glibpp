#include <glib/filestream.hpp>
#include <stdio.h>

using namespace GLib;

FileStream GLib::stdin = FileStream(0, "r");
FileStream GLib::stdout = FileStream(1, "w");
FileStream GLib::stderr = FileStream(2, "w");

void FileStream::free()
{
    if (stream != nullptr)
    {
        fclose((FILE *)stream);
        stream = nullptr;
    }
}

FileStream::FileStream(int fd, string mode)
{
    stream = fdopen(fd, (char *)mode.get_data());
}

FileStream::FileStream(string path, string mode)
{
    stream = fopen((char *)path.get_data(), (char *)mode.get_data());
}

FileStream::~FileStream()
{
    free();
}

FileStream::FileStream(FileStream &&other)
{
    stream = other.stream;
    other.stream = nullptr;
}

FileStream &FileStream::operator=(FileStream &&other)
{
    free();
    stream = other.stream;
    other.stream = nullptr;
    return *this;
}

int FileStream::flush()
{
    return fflush((FILE *)stream);
}

char *FileStream::gets(char *buffer, size len)
{
    return fgets(buffer, len, (FILE *)stream);
}

void FileStream::printf(const string &format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf((FILE *)stream, (char *)format.get_data(), args);
    va_end(args);
}

int FileStream::puts(string str)
{
    return fputs((char *)str.get_data(), (FILE *)stream);
}

int FileStream::scanf(const string &format, ...)
{
    va_list args;
    va_start(args, format);
    int result = vfscanf((FILE *)stream, (char *)format.get_data(), args);
    va_end(args);
    return result;
}

void FileStream::vprintf(const string &format, va_list args)
{
    vfprintf((FILE *)stream, (char *)format.get_data(), args);
}
