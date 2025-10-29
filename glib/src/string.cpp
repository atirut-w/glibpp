#include <glibpp/mem.hpp>
#include <glibpp/string.hpp>
#include <string.h>

namespace GLib {

String::String() {
  maybe_expand(2);
  str[0] = '\0';
}

String::String(const char *init) {
  if (!init || *init == '\0') {
    maybe_expand(2);
    str[0] = '\0';
  } else {
    size init_len = strlen(init);
    maybe_expand(init_len + 2);
    append_len(init, init_len);
  }
}

String::String(const String &other) {
  allocated_len = other.allocated_len;
  len = other.len;

  str = new char[allocated_len];
  if (str && len > 0) {
    memcpy(str, other.str, len);
  }
  str[len] = '\0';
}

String &String::operator=(const String &other) {
  if (this != &other) {
    delete[] str;

    allocated_len = other.allocated_len;
    len = other.len;

    str = new char[allocated_len];
    if (str && len > 0) {
      memcpy(str, other.str, len);
    }
    str[len] = '\0';
  }
  return *this;
}

String::String(String &&other) noexcept
    : str(other.str), len(other.len), allocated_len(other.allocated_len) {
  other.str = nullptr;
  other.len = 0;
  other.allocated_len = 0;
}

String &String::operator=(String &&other) noexcept {
  if (this != &other) {
    delete[] str;

    str = other.str;
    len = other.len;
    allocated_len = other.allocated_len;

    other.str = nullptr;
    other.len = 0;
    other.allocated_len = 0;
  }
  return *this;
}

String::~String() {
  delete[] str;
}

String &String::insert_len(ssize pos, const char *val, ssize val_len) {
  size ulen, upos;

  if (val_len == 0) {
    return *this;
  }

  if (val_len < 0) {
    ulen = strlen(val);
  } else {
    ulen = static_cast<size>(val_len);
  }

  if (pos < 0) {
    upos = len;
  } else {
    upos = static_cast<size>(pos);
  }

  if (val >= str && val <= str + len) {
    size offset = val - str;
    size precount = 0;

    maybe_expand(ulen);
    val = str + offset;

    if (upos < len) {
      memmove(str + upos + ulen, str + upos, len - upos);
    }

    if (offset < upos) {
      precount = ulen < (upos - offset) ? ulen : (upos - offset);
      memcpy(str + upos, val, precount);
    }

    if (ulen > precount) {
      memcpy(str + upos + precount, val + precount + ulen, ulen - precount);
    }
  } else {
    maybe_expand(ulen);

    if (upos < len) {
      memmove(str + upos + ulen, str + upos, len - upos);
    }

    if (ulen == 1) {
      str[upos] = val[0];
    } else {
      memcpy(str + upos, val, ulen);
    }
  }

  len += ulen;
  str[len] = '\0';
  return *this;
}

String &String::append_len(const char *val, ssize val_len) {
  return insert_len(-1, val, val_len);
}

void String::expand(size len) {
  size want_alloc = 1;
  while (want_alloc < this->len + len + 1) {
    want_alloc *= 2;
  }

  char *new_str = new char[want_alloc];
  for (size i = 0; i < this->len; i++) {
    new_str[i] = this->str[i];
  }
  new_str[this->len] = '\0';

  delete[] this->str;
  this->str = new_str;
  this->allocated_len = want_alloc;
}

void String::maybe_expand(size len) {
  if (len >= this->allocated_len - this->len) {
    expand(len);
  }
}

} // namespace GLib
