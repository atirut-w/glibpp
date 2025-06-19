#include "glibpp/math.hpp"
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

String::String(const String &other) {
  len = other.len;
  allocated_len = other.allocated_len;
  str = new char[allocated_len];
  // We can't just strcpy because other.str might have embedded null characters
  std::memcpy(str, other.str, len + 1);
}

String::~String() { delete[] str; }

String &String::operator=(const char *rval) {
  truncate(0);
  append(rval);
  return *this;
}

String &String::append(const char *val) {
  insert_len(-1, val, -1);
  return *this;
}

String &String::insert_len(std::size_t pos, const char *val, std::size_t len) {
  if (val == nullptr && len != 0) {
    return *this;
  }

  if (len == 0) {
    return *this;
  }

  std::size_t len_unsigned, pos_unsigned;

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
    std::size_t offset = val - str;
    std::size_t precount = 0;

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

String &String::truncate(std::size_t new_len) {
  len = min(len, new_len);
  str[len] = '\0';
  return *this;
}

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

} // namespace GLib
