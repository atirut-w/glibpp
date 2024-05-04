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

        operator gboolean() const { return stream != nullptr; }

        gchar *gets(gchar *buffer, gsize len);
        int puts(string str);
    };

    /// @brief The input stream
    extern FileStream stdin;
    /// @brief The output stream
    extern FileStream stdout;
    /// @brief The error stream
    extern FileStream stderr;
}
