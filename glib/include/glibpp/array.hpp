#pragma once
#include <glibpp/config.hpp>x
#include <glibpp/refcount.hpp>
#include <glibpp/types.hpp>

namespace GLib {

template <typename T> class Array {
  struct Control {
    T *data = nullptr;
    size len = 0;
    size capacity = 0;
    AtomicRefCount refcount;
  };

  Control *control = nullptr;

  void maybe_expand(uint len) {
    // TODO: Overflow check

    uint want_len = control->len + len;
    if (want_len > control->capacity) {
      size want_alloc = 1;
      while (want_alloc < want_len) {
        want_alloc *= 2;
      }

      T *new_data = static_cast<T *>(::operator new(sizeof(T) * want_alloc));
      uint i;
      try {
        for (i = 0; i < control->len; i++) {
          new (new_data + i) T(steal(control->data[i]));
        }
      } catch (...) {
        for (uint j = 0; j < i; j++) {
          new_data[j].~T();
        }
        ::operator delete(new_data);
        throw;
      }

      for (uint j = 0; j < control->len; j++) {
        control->data[j].~T();
      }
      ::operator delete(control->data);
      control->data = new_data;
      control->capacity = want_alloc;
    }
  }

public:
  Array() { control = new Control(); }

  Array(const Array &other) : control(other.control) {
    if (control) {
      control->refcount.inc();
    }
  }

  Array &operator=(const Array &other) {
    if (this != &other) {
      if (control && control->refcount.dec()) {
        for (uint i = 0; i < control->len; i++) {
          control->data[i].~T();
        }
        ::operator delete(control->data);
        delete control;
      }
      control = other.control;
      control->refcount.inc();
    }
    return *this;
  }

  Array(Array &&other) noexcept : control(other.control) {
    other.control = nullptr;
  }

  Array &operator=(Array &&other) noexcept {
    if (this != &other) {
      if (control && control->refcount.dec()) {
        for (uint i = 0; i < control->len; i++) {
          control->data[i].~T();
        }
        ::operator delete(control->data);
        delete control;
      }
      control = other.control;
      other.control = nullptr;
    }
    return *this;
  }

  ~Array() {
    if (control && control->refcount.dec()) {
      for (uint i = 0; i < control->len; i++) {
        control->data[i].~T();
      }
      ::operator delete(control->data);
      delete control;
    }
  }

  T *data() { return control->data; }

  size len() const { return control->len; }

  Array &append_val(const T &val) { return append_vals(&val, 1); }

  Array &append_vals(const T *vals, uint length) {
    maybe_expand(length);

    for (uint i = 0; i < length; i++) {
      new (control->data + control->len + i) T(vals[i]);
    }
    control->len += length;

    return *this;
  }

  Array &prepend_val(const T &val) { return prepend_vals(&val, 1); }

  Array &prepend_vals(const T *vals, uint length) {
    maybe_expand(length);

    for (uint i = control->len; i > 0; i--) {
      new (control->data + i + length - 1) T(steal(control->data[i - 1]));
      control->data[i - 1].~T();
    }

    for (uint i = 0; i < length; i++) {
      new (control->data + i) T(vals[i]);
    }
    control->len += length;

    return *this;
  }

  Array &insert_val(uint index, const T &val) {
    return insert_vals(index, &val, 1);
  }

  Array &insert_vals(uint index, const T *vals, uint length) {
    maybe_expand(length);

    for (uint i = control->len; i > index; i--) {
      new (control->data + i + length - 1) T(steal(control->data[i - 1]));
      control->data[i - 1].~T();
    }

    for (uint i = 0; i < length; i++) {
      new (control->data + index + i) T(vals[i]);
    }
    control->len += length;

    return *this;
  }

  Array &set_size(uint length) {
    if (length > control->len) {
      maybe_expand(length - control->len);

      for (uint i = control->len; i < length; i++) {
        new (control->data + i) T();
      }
    } else if (length < control->len) {
      remove_range(length, control->len - length);
    }
    control->len = length;

    return *this;
  }

  Array &remove_index(uint index) { return remove_range(index, 1); }

  Array &remove_range(uint index, uint length) {
    for (uint i = index; i < index + length; i++) {
      control->data[i].~T();
    }

    for (uint i = index + length; i < control->len; i++) {
      new (control->data + i - length) T(steal(control->data[i]));
      control->data[i].~T();
    }
    control->len -= length;

    return *this;
  }

  // TODO: Sort & binary search
};

} // namespace GLib
