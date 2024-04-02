#include <glib.hpp>
#include <cstring>

#define ALLOCATE_STEP 128

string::string(const char *init)
{
    if (init == nullptr)
        return;
    
    len = strlen(init);
    while (allocated_len < len + 1)
    {
        allocated_len += ALLOCATE_STEP;
    }

    str = new gchar[allocated_len];
    memcpy(str, init, len + 1);
}

string::~string()
{
    if (str != nullptr)
        delete[] str;
}

string::string(const string &other)
{
    len = other.len;
    allocated_len = other.allocated_len;
    
    if (other.str != nullptr)
    {
        str = new gchar[allocated_len];
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
        len = other.len;
        allocated_len = other.allocated_len;

        if (other.str != nullptr)
        {
            str = new gchar[allocated_len];
            memcpy(str, other.str, len + 1);
        }
        else
        {
            str = nullptr;
        }
    }
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

const guint8 *string::get_data() const
{
    return (const guint8 *)str;
}

gsize string::get_length() const
{
    return len;
}
