#include <glibpp/object.hpp>
#include <glib-2.0/glib-object.h>

using namespace GLib;

void Object::connect(const char *signal, void (*handler)(), void *data)
{
    g_signal_connect(object, signal, G_CALLBACK(handler), data);
}
