#pragma once
#include <glib/primitives.hpp>

namespace GLib
{
    class string
    {
    private:
        char *str = nullptr;
        size len = 0;

        void free();

    public:
        string() = default;
        string(const char *str);
        ~string();

        string(const string &other);
        string &operator=(const string &other);
        string(string &&other);
        string &operator=(string &&other);

        const uint8 *get_data() const;
        size get_length() const;

        bool operator==(const string &other) const;
        bool operator!=(const string &other) const;

        uint hash() const;
    };
}
