#pragma once
#include <glib/glib/file_stream.hpp>

namespace GLib
{
    /// @brief The input stream
    extern FileStream stdin;
    /// @brief The output stream
    extern FileStream stdout;
    /// @brief The error stream
    extern FileStream stderr;
}
