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
