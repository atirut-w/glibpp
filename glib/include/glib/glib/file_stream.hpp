#pragma once
#include <glib/string.hpp>
#include <glib/glib/pointer.hpp>

namespace GLib
{
    class FileStream
    {
    private:
        FileStream() = default;
        void *stream = nullptr;

    public:
        static Pointer<FileStream> open(gint fd, string mode);
        static Pointer<FileStream> open(string path, string mode);
        ~FileStream();

        string gets(gchar *buffer, gsize len);
        int puts(string str);
    };
}
