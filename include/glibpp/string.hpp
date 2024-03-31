#pragma once
#include <stddef.h>

namespace GLib
{
    class String
    {
    private:
        char *str = nullptr;
        size_t len = 0;
        size_t allocated = 0;
    public:
        String() = default;
        String(const char *str);
        String(const String &other);
        ~String();

        String &operator=(const String &other);
    };
}
