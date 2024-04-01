#pragma once

typedef char gchar;
typedef unsigned long gsize;
typedef unsigned char guint8;

class string
{
private:
    gchar *str = nullptr;
    gsize len = 0;
    gsize allocated_len = 0;

public:
    string() = default;
    string(const char *str);
    string(const string &other);
    ~string();

    string &operator=(const string &other);

    const guint8 *get_data() const;
    gsize get_length() const;
};
