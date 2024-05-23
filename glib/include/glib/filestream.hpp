#pragma once
#include <glib/string.hpp>
#include <glib/primitives.hpp>
#include <cstdarg>

namespace GLib
{
    class FileStream
    {
    private:
        FileStream() = default;
        void *stream = nullptr;

        void free();

    public:
        FileStream(int fd, string mode);
        FileStream(string path, string mode);
        ~FileStream();

        FileStream(const FileStream &other) = delete;
        FileStream &operator=(const FileStream &other) = delete;
        FileStream(FileStream &&other);
        FileStream &operator=(FileStream &&other);

        operator bool() const { return stream != nullptr; }

        int flush();
        char *gets(char *buffer, size len);
        void printf(const string &format, ...);
        int puts(string str);
        int scanf(const string &format, ...);
        void vprintf(const string &format, va_list args);
    };

    /// @brief The input stream
    extern FileStream stdin;
    /// @brief The output stream
    extern FileStream stdout;
    /// @brief The error stream
    extern FileStream stderr;
}
