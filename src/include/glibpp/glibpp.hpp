#pragma once

namespace GLib
{
    enum ApplicationFlags
    {
        NONE = 0,
    };

    class Object
    {
    public:
        void *object;
        void connect(const char *signal, void (*handler)(), void *data);
    };

    class Application : public Object
    {
    public:
        int run(int argc, char **argv);
    };
}
