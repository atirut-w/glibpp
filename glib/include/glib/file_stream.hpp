#pragma once
#include <glib/string.hpp>
#include <cstddef>

namespace GLib
{
    class FileStream
    {
    private:
        FileStream() = default;
        void *stream = nullptr;

    public:
        FileStream(int fd, string mode);
        FileStream(string path, string mode);
        ~FileStream();

        operator bool() const { return stream != nullptr; }

        char *gets(char *buffer, size_t len);
        int puts(string str);
    };

    /// @brief The input stream
    extern FileStream stdin;
    /// @brief The output stream
    extern FileStream stdout;
    /// @brief The error stream
    extern FileStream stderr;
}
