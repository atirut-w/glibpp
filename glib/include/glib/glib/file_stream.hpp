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
        static owned<FileStream> open(gint fd, string mode);
        static owned<FileStream> open(string path, string mode);
        ~FileStream();

        unowned<string> gets(gchar *buffer, gsize len);
        int puts(string str);
    };
}
