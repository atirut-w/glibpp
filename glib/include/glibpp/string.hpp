#pragma once
#include <cstdarg>
#include <cstddef>

namespace GLib {

struct String {
  char *str;
  std::size_t len;
  std::size_t allocated_len;

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
  String &append_len(const char *val, std::size_t len);
  String &append_printf(const char *format, ...);
  String &append_vprintf(const char *format, std::va_list args);
  String &assign(const char *rval);
  bool equal(const String &other) const;
  unsigned int hash() const;
  String &insert(std::size_t pos, const char *val);
  String &insert_c(std::size_t pos, char c);
  String &insert_len(std::size_t pos, const char *val, std::size_t len);
  // TODO: insert_unichar
  String &printf(const char *format, ...);
  String &truncate(std::size_t new_len);

private:
  void expand(std::size_t size);
  void maybe_expand(std::size_t size);
};

} // namespace GLib
