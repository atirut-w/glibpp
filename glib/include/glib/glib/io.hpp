#pragma once
#include <glib/glib/file_stream.hpp>

namespace GLib
{
    /// @brief The input stream
    extern owned<FileStream> stdin;
    /// @brief The output stream
    extern owned<FileStream> stdout;
    /// @brief The error stream
    extern owned<FileStream> stderr;
}
