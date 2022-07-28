#include <glibpp/glibpp.hpp>
#include <gio/gio.h>

using namespace GLib;

void Object::connect(const char *signal, void (*handler)(), void *data)
{
    g_signal_connect(this->object, signal, handler, data);
}

int Application::run(int argc, char **argv)
{
    return g_application_run(G_APPLICATION(object), argc, argv);
}
