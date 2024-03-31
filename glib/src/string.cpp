#include <glib.hpp>
#include <cstring>

#define ALLOCATE_STEP 128

string::string(const char *init)
{
    len = strlen(init);
    while (allocated_len < len + 1)
    {
        allocated_len += ALLOCATE_STEP;
    }

    str = new char[allocated_len];
    memcpy(str, init, len + 1);
}

string::string(const string &other)
{
    len = other.len;
    allocated_len = other.allocated_len;
    str = new char[allocated_len];
    memcpy(str, other.str, len + 1);
}

string::~string()
{
    delete[] str;
}

string &string::operator=(const string &other)
{
    if (this != &other)
    {
        delete[] str;
        len = other.len;
        allocated_len = other.allocated_len;
        str = new char[allocated_len];
        memcpy(str, other.str, len + 1);
    }
    return *this;
}

const char *string::get_data() const
{
    return str;
}

size_t string::get_length() const
{
    return len;
}
