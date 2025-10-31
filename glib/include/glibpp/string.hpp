#pragma once
#include <glibpp/config.hpp>
#include <glibpp/types.hpp>
#include <stdarg.h> // TODO: Replace this with our own va_list

namespace GLib {

struct String {
  char *str = nullptr;
  size len = 0;
  size allocated_len = 0;

  String();
  String(const char *init);

  String(const String &other);
  String &operator=(const String &other);
  String(String &&other) noexcept;
  String &operator=(String &&other) noexcept;
  
  ~String();

  // TODO: Steal and free to bytes

  bool equal(const String &other) const;
  String &assign(const char *rval);
  String &truncate(size new_len);
  String &set_size(size new_len);
  String &insert_len(ssize pos, const char *val, ssize val_len);
  String &append(const char *val);
  String &append_len(const char *val, ssize val_len);
  String &append_c(char c);
  
  // TODO: Append unichar

  String &prepend(const char *val);
  String &prepend_c(char c);

  // TODO: Prepend unichar

  String &prepend_len(const char *val, ssize val_len);
  String &insert(ssize pos, const char *val);
  String &insert_c(ssize pos, char c);

  // TODO: Insert unichar

  String &overwrite(ssize pos, const char *val);
  String &overwrite_len(ssize pos, const char *val, ssize val_len);
  String &erase(ssize pos, ssize len);
  uint replace(const char *find, const char *replace, uint limit);
  String &ascii_down();
  String &ascii_up();
  void vprintf(const char *format, va_list args);
  void printf(const char *format, ...);
  void append_vprintf(const char *format, va_list args);
  void append_printf(const char *format, ...);
  
  // TODO: Append URI escaped

private:
  void expand(size len);
  void maybe_expand(size len);
};

// TODO: String hash specialization

} // namespace GLib
