#include <cstddef>
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

String &String::operator=(const char *rval) {
  std::size_t new_len = std::strlen(rval);
  if (new_len >= allocated_len) {
    delete[] str;
    allocated_len = new_len + 1; // +1 for null terminator
    str = new char[allocated_len];
  }
  std::strcpy(str, rval);
  len = new_len;
  return *this;
}

} // namespace GLib
