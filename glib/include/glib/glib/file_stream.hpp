#pragma once
#include <glib/string.hpp>

namespace GLib
{
    class FileStream
    {
    private:
        FileStream() = default;
        void *stream = nullptr;

    public:
        FileStream(gint fd, string mode);
        FileStream(string path, string mode);
        ~FileStream();

        gchar *gets(gchar *buffer, gsize len);
        int puts(string str);
    };
}
