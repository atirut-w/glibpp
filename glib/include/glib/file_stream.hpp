#pragma once
#include <glib/string.hpp>
#include <glib/primitives.hpp>

namespace GLib
{
    class FileStream
    {
    private:
        FileStream() = default;
        void *stream = nullptr;

        void free();

    public:
        FileStream(gint fd, string mode);
        FileStream(string path, string mode);
        ~FileStream();

        FileStream(const FileStream &other) = delete;
        FileStream &operator=(const FileStream &other) = delete;
        FileStream(FileStream &&other);
        FileStream &operator=(FileStream &&other);

        operator bool() const { return stream != nullptr; }

        gchar *gets(gchar *buffer, gsize len);
        gint puts(string str);
    };

    /// @brief The input stream
    extern FileStream stdin;
    /// @brief The output stream
    extern FileStream stdout;
    /// @brief The error stream
    extern FileStream stderr;
}
