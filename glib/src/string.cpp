#include "glibpp/math.hpp"
#include <cstddef>
#include <cstring>
#include <glibpp/string.hpp>

namespace GLib {

void String::expand(std::size_t size) {
  allocated_len = len + size + 1;
  char *new_str = new char[allocated_len];
  std::memcpy(new_str, str, len);
  new_str[len] = '\0';
  delete[] str;
  str = new_str;
}

void String::maybe_expand(std::size_t size) {
  if (len + size >= allocated_len) {
    expand(size);
  }
}

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

String::String(const String &other) {
  len = other.len;
  allocated_len = other.allocated_len;
  str = new char[allocated_len];
  // We can't just strcpy because other.str might have embedded null characters
  std::memcpy(str, other.str, len + 1);
}

String::~String() { delete[] str; }

String &String::operator=(const char *rval) {
  std::size_t new_len = std::strlen(rval);
  maybe_expand(new_len - len);
  std::strcpy(str, rval);
  len = new_len;
  return *this;
}

String &String::append(const char *val) {
  std::size_t val_len = std::strlen(val);
  maybe_expand(val_len);
  std::strcpy(str + len, val);
  len += val_len;
  return *this;
}

String &String::truncate(std::size_t new_len) {
  len = min(len, new_len);
  str[len] = '\0';
  return *this;
}

} // namespace GLib
