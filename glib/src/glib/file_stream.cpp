#include <glib/glib/file_stream.hpp>
#include <stdio.h>

using namespace GLib;

owned<FileStream> FileStream::open(gint fd, string mode)
{
    owned<FileStream> fs = new FileStream();
    fs->stream = fdopen(fd, (char *)mode.get_data());

    if (fs->stream == nullptr)
    {
        return nullptr;
    }
    return fs;
}

owned<FileStream> FileStream::open(string path, string mode)
{
    owned<FileStream> fs = new FileStream();
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

string FileStream::gets(gchar *buffer, gsize len)
{
    return fgets(buffer, len, (FILE *)stream);
}

int FileStream::puts(string str)
{
    if (str == nullptr)
        return 0;
    return fputs((char *)str.get_data(), (FILE *)stream);
}
