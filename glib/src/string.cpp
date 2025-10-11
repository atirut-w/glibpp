#include "glibpp/math.hpp"
#include <cstdarg>
#include <cstddef>
#include <cstdio>
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

String::String(const String &other) {
  len = other.len;
  allocated_len = other.allocated_len;
  str = new char[allocated_len];
  // We can't just strcpy because other.str might have embedded null characters
  std::memcpy(str, other.str, len + 1);
}

String::~String() { delete[] str; }

String &String::operator=(const char *rval) {
  return assign(rval);
}

String String::operator+(const char *val) const {
  String result(*this);
  result.append(val);
  return result;
}

String &String::operator+=(const char *val) {
  return append(val);
}

String &String::operator+=(char c) {
  return append_c(c);
}

bool String::operator==(const String &other) const {
  return equal(other);
}

bool String::operator!=(const String &other) const {
  return !equal(other);
}

String &String::append(const char *val) {
  return insert_len(-1, val, -1);
}

String &String::append_c(char c) {
  return insert_c(-1, c);
}

String &String::append_len(const char *val, size len) {
  return insert_len(-1, val, len);
}

String &String::append_printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  String &result = append_vprintf(format, args);
  va_end(args);
  return result;
}

String &String::append_vprintf(const char *format, std::va_list args) {
  va_list args_copy;
  va_copy(args_copy, args);
  int size = std::vsnprintf(nullptr, 0, format, args_copy);
  va_end(args_copy);
  if (size < 0) {
    // TODO: Throw?
    return *this;
  }

  maybe_expand(size);

  std::vsnprintf(str + len, size + 1, format, args);
  len += size;
  str[len] = '\0';

  return *this;
}

String &String::assign(const char *rval) {
  truncate(0);
  append(rval);
  return *this;
}

bool String::equal(const String &other) const {
  char *p = str;
  char *q = other.str;
  size i = len;

  if (len != other.len) {
    return false;
  }

  while (i) {
    if (*p != *q) {
      return false;
    }
    p++;
    q++;
    i--;
  }

  return true;
}

unsigned int String::hash() const {
  const char *p = str;
  size n = len;
  unsigned int h = 0;

  while (n--) {
    h = (h << 5) - h + static_cast<unsigned char>(*p++);
  }

  return h;
}

String &String::insert(size pos, const char *val) {
  return insert_len(pos, val, -1);
}

String &String::insert_c(size pos, char c) {
  maybe_expand(1);

  size pos_unsigned;
  if (pos == -1) {
    pos_unsigned = this->len;
  } else {
    pos_unsigned = pos;
    if (pos_unsigned > this->len) {
      return *this;
    }
  }

  if (pos_unsigned < this->len) {
    std::memmove(str + pos_unsigned + 1, str + pos_unsigned,
                 this->len - pos_unsigned);
  }

  str[pos_unsigned] = c;
  this->len += 1;
  str[this->len] = '\0';
  return *this;
}

String &String::insert_len(size pos, const char *val, size len) {
  if (val == nullptr && len != 0) {
    return *this;
  }

  if (len == 0) {
    return *this;
  }

  size len_unsigned, pos_unsigned;

  if (len == -1) {
    len_unsigned = std::strlen(val);
  } else {
    len_unsigned = len;
  }

  if (pos == -1) {
    pos_unsigned = this->len;
  } else {
    pos_unsigned = pos;
    if (pos_unsigned > this->len) {
      return *this;
    }
  }

  if (val >= str && val <= str + this->len) {
    size offset = val - str;
    size precount = 0;

    maybe_expand(len_unsigned);
    val = str + offset;

    if (pos_unsigned < this->len) {
      std::memmove(str + pos_unsigned + len_unsigned, str + pos_unsigned,
                   this->len - pos_unsigned);
    }

    if (offset < pos_unsigned) {
      precount = min(len_unsigned, pos_unsigned - offset);
      std::memcpy(str + pos_unsigned, val, precount);
    }

    if (len_unsigned > precount) {
      std::memcpy(str + pos_unsigned + precount, val + precount + len_unsigned,
                  len_unsigned - precount);
    }
  } else {
    maybe_expand(len_unsigned);

    if (pos_unsigned < this->len) {
      std::memmove(str + pos_unsigned + len_unsigned, str + pos_unsigned,
                   this->len - pos_unsigned);
    }

    if (len_unsigned == 1) {
      str[pos_unsigned] = *val;
    } else {
      std::memcpy(str + pos_unsigned, val, len_unsigned);
    }
  }

  this->len += len_unsigned;
  str[this->len] = '\0';

  return *this;
}

String &String::printf(const char *format, ...) {
  truncate(0);
  va_list args;
  va_start(args, format);
  String &result = append_vprintf(format, args);
  va_end(args);
  return result;
}

String &String::truncate(size new_len) {
  len = min(len, new_len);
  str[len] = '\0';
  return *this;
}

void String::expand(size size) {
  allocated_len = len + size + 1;
  char *new_str = new char[allocated_len];
  std::memcpy(new_str, str, len);
  new_str[len] = '\0';
  delete[] str;
  str = new_str;
}

void String::maybe_expand(size size) {
  if (len + size >= allocated_len) {
    expand(size);
  }
}

} // namespace GLib
