#pragma once
#include <glibpp/object.hpp>

namespace GLib
{
    class Application : public Object
    {
    public:
        int run(int argc, char **argv);
    };
}
