#pragma once
#include <stddef.h>

class string
{
private:
    char *str = nullptr;
    size_t len = 0;
    size_t allocated = 0;

public:
    string() = default;
    string(const char *str);
    string(const string &other);
    ~string();

    string &operator=(const string &other);
};
