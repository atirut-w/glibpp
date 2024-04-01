#pragma once
#include <glib/global.hpp>

namespace GLib
{
    class FileStream
    {
    private:
        FileStream() = default;
        void *stream = nullptr;

    public:
        static FileStream *open(gint fd, string mode);
        static FileStream *open(string path, string mode);
        ~FileStream();
    };
}
