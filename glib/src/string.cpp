#include <cstring>
#include <glibpp/string.hpp>

namespace GLib {

String::String(const char *init) {
  if (init == nullptr || *init == '\0') {
    str = new char[1];
    str[0] = '\0';
    len = 0;
    allocated_len = 1;
  } else {
    len = std::strlen(init);
    allocated_len = len + 1; // +1 for null terminator
    str = new char[allocated_len];
    std::strcpy(str, init);
  }
}

String::~String() { delete[] str; }

} // namespace GLib
