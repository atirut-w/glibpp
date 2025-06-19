#pragma once
#include <cstddef>

namespace GLib {

class String {
  char *str;
  std::size_t len;
  std::size_t allocated_len;

  void expand(std::size_t size);
  void maybe_expand(std::size_t size);

public:
  String(const char *init);
  String(const String &other);
  ~String();

  String &operator=(const String &other) = delete;
  String &operator=(const char *rval);

  String &append(const char *val);
  String &insert_len(std::size_t pos, const char *val, std::size_t len);
  String &truncate(std::size_t new_len);
};

} // namespace GLib
