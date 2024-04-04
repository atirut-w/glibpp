#pragma once
#include <glib/primitives.hpp>

class string
{
private:
    gchar *str = nullptr;
    gsize len = 0;

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

    const guint8 *get_data() const;
    gsize get_length() const;
};
