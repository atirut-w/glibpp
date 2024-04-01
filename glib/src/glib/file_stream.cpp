#include <glib.hpp>
#include <stdio.h>

using namespace GLib;

FileStream *FileStream::open(gint fd, string mode)
{
    FileStream *fs = new FileStream();
    fs->stream = fdopen(fd, (char *)mode.get_data());

    if (fs->stream == nullptr)
    {
        delete fs;
        return nullptr;
    }
    return fs;
}

FileStream *FileStream::open(string path, string mode)
{
    FileStream *fs = new FileStream();
    fs->stream = fopen((char *)path.get_data(), (char *)mode.get_data());

    if (fs->stream == nullptr)
    {
        delete fs;
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
    return fputs((char *)str.get_data(), (FILE *)stream);
}
