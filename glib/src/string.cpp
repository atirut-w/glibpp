#include <glib.hpp>
#include <cstring>

using namespace GLib;

#define ALLOCATE_STEP 128

void string::free()
{
    if (str != nullptr)
        delete[] str;
    str = nullptr;
    len = 0;
}

string::string(const char *init)
{
    if (init == nullptr)
        return;
    
    len = strlen(init);
    str = new char[len + 1];
    memcpy(str, init, len + 1);
}

string::~string()
{
    free();
}

string::string(const string &other)
{
    len = other.len;
    if (other.str != nullptr)
    {
        str = new char[len + 1];
        memcpy(str, other.str, len + 1);
    }
    else
    {
        str = nullptr;
    }
}

string &string::operator=(const string &other)
{
    if (this != &other)
    {
        free();
        
        len = other.len;
        if (other.str != nullptr)
        {
            str = new char[len + 1];
            memcpy(str, other.str, len + 1);
        }
        else
        {
            str = nullptr;
        }
    }
    return *this;
}

string::string(string &&other)
{
    str = other.str;
    len = other.len;
    other.str = nullptr;
    other.len = 0;
}

string &string::operator=(string &&other)
{
    free();
    
    str = other.str;
    len = other.len;
    other.str = nullptr;
    other.len = 0;
    return *this;
}

bool string::operator==(const string &other) const
{
    if (str == other.str)
        return true;
    if (len != other.len)
        return false;
    return memcmp(str, other.str, len) == 0;
}

bool string::operator!=(const string &other) const
{
    if (str == other.str)
        return false;
    if (len != other.len)
        return true;
    return memcmp(str, other.str, len) != 0;
}

const guint8 *string::get_data() const
{
    return (const guint8 *)str;
}

size_t string::get_length() const
{
    return len;
}
