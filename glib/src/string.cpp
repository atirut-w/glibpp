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

bool String::equal(const String &other) const {
  size i = other.len;
  if (i != other.len) {
    return false;
  }
  
  char *p = str;
  char *q = other.str;

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

String &String::assign(const char *rval) {
  truncate(0);
  return append(rval);
}

String &String::truncate(size new_len) {
  len = new_len < len ? new_len : len;
  str[len] = '\0';
  return *this;
}

String &String::set_size(size new_len) {
  if (new_len > allocated_len) {
    maybe_expand(new_len - len);
  }

  len = new_len;
  str[len] = '\0';

  return *this;
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

String &String::append(const char *val) {
  return insert_len(-1, val, -1);
}

String &String::append_len(const char *val, ssize val_len) {
  return insert_len(-1, val, val_len);
}

String &String::append_c(char c) {
  return insert_c(-1, c);
}

String &String::prepend(const char *val) {
  return insert_len(0, val, -1);
}

String &String::prepend_c(char c) {
  return insert_c(0, c);
}

String &String::prepend_len(const char *val, ssize val_len) {
  return insert_len(0, val, val_len);
}

String &String::insert(ssize pos, const char *val) {
  return insert_len(pos, val, -1);
}

String &String::insert_c(ssize pos, char c) {
  maybe_expand(1);

  size upos;
  if (pos < 0) {
    upos = len;
  } else {
    upos = static_cast<size>(pos);
  }

  if (upos < len) {
    memmove(str + upos + 1, str + upos, len - upos);
  }
  str[upos] = c;
  len++;
  str[len] = '\0';

  return *this;
}

String &String::overwrite(ssize pos, const char *val) {
  return overwrite_len(pos, val, strlen(val));
}

String &String::overwrite_len(ssize pos, const char *val, ssize val_len) {
  if (!len) {
    return *this;
  }

  size ulen, end;
  if (val_len < 0) {
    ulen = strlen(val);
  } else {
    ulen = static_cast<size>(val_len);
  }
  end = static_cast<size>(pos) + ulen;

  if (end > len) {
    maybe_expand(end - len);
  }

  memcpy(str + static_cast<size>(pos), val, ulen);

  if (end > len) {
    str[len] = '\0';
    len = end;
  }

  return *this;
}

String &String::erase(ssize pos, ssize len) {
  size ulen, upos;
  upos = static_cast<size>(pos);

  if (len < 0) {
    ulen = this->len - upos;
  } else {
    ulen = static_cast<size>(len);

    if (!(upos + ulen <= this->len)) {
      return *this;
    }

    if (upos + ulen < this->len) {
      memmove(str + upos, str + upos + ulen, this->len - (upos + ulen));
    }
  }

  this->len -= ulen;
  str[this->len] = '\0';
  return *this;
}

uint String::replace(const char *find, const char *replace, uint limit) {
  return 0; // TODO: implement
}

String &String::ascii_down() {
  return *this; // TODO: implement
}

String &String::ascii_up() {
  return *this; // TODO: implement
}

void String::vprintf(const char *format, va_list args) {
  return; // TODO: implement
}

void String::printf(const char *format, ...) {
  return; // TODO: implement
}

void String::append_vprintf(const char *format, va_list args) {
  return; // TODO: implement
}

void String::append_printf(const char *format, ...) {
  return; // TODO: implement
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
