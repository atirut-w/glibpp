#include <glib/file_stream.hpp>
#include <stdio.h>

using namespace GLib;

FileStream::FileStream(gint fd, string mode)
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

gchar *FileStream::gets(gchar *buffer, gsize len)
{
    return fgets(buffer, len, (FILE *)stream);
}

int FileStream::puts(string str)
{
    return fputs((char *)str.get_data(), (FILE *)stream);
}