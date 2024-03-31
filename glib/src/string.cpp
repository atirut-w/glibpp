#include <glib.hpp>
#include <cstring>

#define ALLOCATE_STEP 128

string::string(const char *init)
{
    len = strlen(init);
    while (allocated < len + 1)
    {
        allocated += ALLOCATE_STEP;
    }

    str = new char[allocated];
    memcpy(str, init, len + 1);
}

string::string(const string &other)
{
    len = other.len;
    allocated = other.allocated;
    str = new char[allocated];
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
        allocated = other.allocated;
        str = new char[allocated];
        memcpy(str, other.str, len + 1);
    }
    return *this;
}
