#include <glibpp/application.hpp>
#include <glib-2.0/gio/gio.h>

using namespace GLib;

int Application::run(int argc, char **argv)
{
    return g_application_run(G_APPLICATION(object), argc, argv);
}
