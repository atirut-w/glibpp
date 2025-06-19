#pragma once
#include <cstddef>

namespace GLib {

class String {
  char *str;
  std::size_t len;
  std::size_t allocated_len;

public:
  String(const char *init);
  String(const String &other);
  ~String();

  String &operator=(const String &other) = delete;
  String &operator=(const char *rval);
};

} // namespace GLib
