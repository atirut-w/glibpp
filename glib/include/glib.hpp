#pragma once
#include <stddef.h>

class string
{
private:
    char *str = nullptr;
    size_t len = 0;
    size_t allocated_len = 0;

public:
    string() = default;
    string(const char *str);
    string(const string &other);
    ~string();

    string &operator=(const string &other);

    const char *get_data() const;
    size_t get_length() const;
};