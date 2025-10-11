#pragma once
#include <cstdarg>
#include <glibpp/config.h>

namespace GLib {

struct String {
  char *str;
  size len;
  size allocated_len;

  String(const char *init);
  String(const String &other);
  ~String();

  String &operator=(const String &other) = delete;
  String &operator=(const char *rval);

  String operator+(const char *val) const;
  String &operator+=(const char *val);
  String &operator+=(char c);

  bool operator==(const String &other) const;
  bool operator!=(const String &other) const;

  String &append(const char *val);
  String &append_c(char c);
  String &append_len(const char *val, size len);
  String &append_printf(const char *format, ...);
  String &append_vprintf(const char *format, std::va_list args);
  String &assign(const char *rval);
  bool equal(const String &other) const;
  unsigned int hash() const;
  String &insert(size pos, const char *val);
  String &insert_c(size pos, char c);
  String &insert_len(size pos, const char *val, size len);
  // TODO: insert_unichar
  String &printf(const char *format, ...);
  String &truncate(size new_len);

private:
  void expand(size size);
  void maybe_expand(size size);
};

} // namespace GLib
