#pragma once
#include <cstddef>
#include <cstdint>

namespace GLib
{
    class string
    {
    private:
        char *str = nullptr;
        size_t len = 0;

        void free();

    public:
        string() = default;
        string(const char *str);
        ~string();

        string(const string &other);
        string &operator=(const string &other);
        string(string &&other);
        string &operator=(string &&other);

        bool operator==(const string &other) const;
        bool operator!=(const string &other) const;

        const uint8_t *get_data() const;
        size_t get_length() const;
    };
}
