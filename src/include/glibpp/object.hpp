#pragma once

namespace GLib
{
    class Object
    {
    public:
        void *object;
        void connect(const char *signal, void (*handler)(), void *data);
    };
}
