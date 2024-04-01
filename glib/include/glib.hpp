#pragma once

typedef unsigned long size_t;
typedef unsigned char uint8;

class string
{
private:
    uint8 *str = nullptr;
    size_t len = 0;
    size_t allocated_len = 0;

public:
    string() = default;
    string(const char *str);
    string(const string &other);
    ~string();

    string &operator=(const string &other);

    const uint8 *get_data() const;
    size_t get_length() const;
};
