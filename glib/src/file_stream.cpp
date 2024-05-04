#include <glib/file_stream.hpp>
#include <stdio.h>

using namespace GLib;

FileStream GLib::stdin = FileStream(0, "r");
FileStream GLib::stdout = FileStream(1, "w");
FileStream GLib::stderr = FileStream(2, "w");

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
    if (stream != nullptr)
    {
        fclose((FILE *)stream);
    }
}

char *FileStream::gets(char *buffer, size_t len)
{
    return fgets(buffer, len, (FILE *)stream);
}

int FileStream::puts(string str)
{
    return fputs((char *)str.get_data(), (FILE *)stream);
}
