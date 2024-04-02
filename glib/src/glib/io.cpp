#include <glib/glib/io.hpp>

using namespace GLib;

owned<FileStream> GLib::stdin = FileStream::open(0, "r");
owned<FileStream> GLib::stdout = FileStream::open(1, "w");
owned<FileStream> GLib::stderr = FileStream::open(2, "w");
