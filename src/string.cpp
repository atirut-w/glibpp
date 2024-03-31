#include <glibpp/string.hpp>
#include <cstring>

#define ALLOCATE_STEP 128

using namespace GLib;

String::String(const char *init)
{
    len = strlen(init);
    while (allocated < len + 1)
    {
        allocated += ALLOCATE_STEP;
    }

    str = new char[allocated];
    memcpy(str, init, len + 1);
}

String::String(const String &other)
{
    len = other.len;
    allocated = other.allocated;
    str = new char[allocated];
    memcpy(str, other.str, len + 1);
}

String::~String()
{
    delete[] str;
}

String &String::operator=(const String &other)
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
