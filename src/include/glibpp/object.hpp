#pragma once
#include <glibpp/closure.hpp>

namespace GLib
{
    class Object
    {
    public:
        void *object;
        void connect(const char *signal, Callback handler, void *data);
    };
}
