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
