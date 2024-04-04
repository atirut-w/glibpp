#include <glib/glib/io.hpp>

using namespace GLib;

FileStream GLib::stdin = FileStream(0, "r");
FileStream GLib::stdout = FileStream(1, "w");
FileStream GLib::stderr = FileStream(2, "w");
