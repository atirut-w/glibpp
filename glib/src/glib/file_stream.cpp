#include <glib.hpp>
#include <stdio.h>

using namespace GLib;

Pointer<FileStream> FileStream::open(gint fd, string mode)
{
    Pointer<FileStream> fs = new FileStream();
    fs->stream = fdopen(fd, (char *)mode.get_data());

    if (fs->stream == nullptr)
    {
        return nullptr;
    }
    return fs;
}

Pointer<FileStream> FileStream::open(string path, string mode)
{
    Pointer<FileStream> fs = new FileStream();
    fs->stream = fopen((char *)path.get_data(), (char *)mode.get_data());

    if (fs->stream == nullptr)
    {
        return nullptr;
    }
    return fs;
}

FileStream::~FileStream()
{
    if (stream != nullptr)
    {
        fclose((FILE *)stream);
    }
}

int FileStream::puts(string str)
{
    if (str == nullptr)
        return 0;
    return fputs((char *)str.get_data(), (FILE *)stream);
}
