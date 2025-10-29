#pragma once
#include <glibpp/config.hpp>
#include <glibpp/mem.hpp>
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

      T *new_data = static_cast<T *>(::operator new[](sizeof(T) * want_alloc));
      uint i;
      try {
        for (i = 0; i < control->len; i++) {
          new (new_data + i) T(GLib::steal(control->data[i]));
        }
      } catch (...) {
        for (uint j = 0; j < i; j++) {
          new_data[j].~T();
        }
        ::operator delete[](new_data);
        throw;
      }

      for (uint j = 0; j < control->len; j++) {
        control->data[j].~T();
      }
      ::operator delete[](control->data);
      control->data = new_data;
      control->capacity = want_alloc;
    }
  }

public:
  Array() { control = new Control(); }

  explicit Array(T *data, uint len) {
    control = new Control();
    control->data = data;
    control->len = len;
    control->capacity = len;
  }

  explicit Array(uint reserved) {
    control = new Control();

    if (reserved) {
      maybe_expand(reserved);
    }
  }

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
        ::operator delete[](control->data);
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
        ::operator delete[](control->data);
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
      ::operator delete[](control->data);
      delete control;
    }
  }

  T *data() { return control->data; }

  size len() const { return control->len; }

  T *steal(size &len) {
    if (!control->refcount.compare(1)) {
      return nullptr;
    }

    T *data = control->data;
    len = control->len;

    control->data = nullptr;
    control->len = 0;
    control->capacity = 0;
    return data;
  }

  Array copy() const {
    Array new_array;
    new_array.append_vals(control->data, control->len);
    return new_array;
  }

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
      new (control->data + i + length - 1) T(GLib::steal(control->data[i - 1]));
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
      new (control->data + i + length - 1) T(GLib::steal(control->data[i - 1]));
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
      control->len = length;
    } else if (length < control->len) {
      remove_range(length, control->len - length);
    }

    return *this;
  }

  Array &remove_index(uint index) { return remove_range(index, 1); }

  Array &remove_range(uint index, uint length) {
    for (uint i = index; i < index + length; i++) {
      control->data[i].~T();
    }

    for (uint i = index + length; i < control->len; i++) {
      new (control->data + i - length) T(GLib::steal(control->data[i]));
      control->data[i].~T();
    }
    control->len -= length;

    return *this;
  }

  // TODO: Sort & binary search

  T &operator[](uint index) { return control->data[index]; }
};

template <typename T> class PtrArray {
  struct Control {
    T **data = nullptr;
    size len = 0;
    size alloc = 0;
    AtomicRefCount refcount;
    bool null_terminated = false;
  };

  Control *control = nullptr;

  void maybe_expand(uint len) {
    uint want_len = control->len + len + (control->null_terminated ? 1 : 0);
    if (want_len > control->alloc) {
      size want_alloc = 1;
      while (want_alloc < want_len) {
        want_alloc *= 2;
      }

      T **new_data = new T *[want_alloc];
      for (uint i = 0; i < control->len; i++) {
        new_data[i] = control->data[i];
      }
      delete[] control->data;
      control->data = new_data;
      control->alloc = want_alloc;
    }
  }

  void maybe_null_terminate() {
    if (control->null_terminated) {
      control->data[control->len] = nullptr;
    }
  }

  explicit PtrArray(uint reserved, bool null_terminated) {
    control = new Control();
    control->null_terminated = null_terminated;

    if (reserved) {
      maybe_expand(reserved);
      if (null_terminated) {
        control->data[0] = nullptr;
      }
    }
  }

public:
  PtrArray() { control = new Control(); }

  explicit PtrArray(uint reserved) : PtrArray(reserved, false) {}

  explicit PtrArray(T **data, uint len) : PtrArray(0, false) {
    control->data = data;
    control->len = len;
    control->alloc = len;
  }

  explicit PtrArray(T **data) {
    size len = 0;

    if (data) {
      while (data[len] != nullptr) {
        len++;
      }
    }

    control = new Control();
    control->data = data;
    control->len = len;
    control->alloc = len;
    control->null_terminated = true;
  }

  // TODO: Variants to construct from Array<T *>?

  PtrArray(const PtrArray &other) : control(other.control) {
    if (control) {
      control->refcount.inc();
    }
  }

  PtrArray &operator=(const PtrArray &other) {
    if (this != &other) {
      if (control && control->refcount.dec()) {
        for (uint i = 0; i < control->len; i++) {
          delete control->data[i];
        }
        delete[] control->data;
        delete control;
      }
      control = other.control;
      control->refcount.inc();
    }
    return *this;
  }

  PtrArray(PtrArray &&other) noexcept : control(other.control) {
    other.control = nullptr;
  }

  PtrArray &operator=(PtrArray &&other) noexcept {
    if (this != &other) {
      if (control && control->refcount.dec()) {
        for (uint i = 0; i < control->len; i++) {
          delete control->data[i];
        }
        delete[] control->data;
        delete control;
      }
      control = other.control;
      other.control = nullptr;
    }
    return *this;
  }

  ~PtrArray() {
    if (control && control->refcount.dec()) {
      for (uint i = 0; i < control->len; i++) {
        delete control->data[i];
      }
      delete[] control->data;
      delete control;
    }
  }

  T **data() { return control->data; }

  size len() const { return control->len; }

  T **steal(size &len) {
    if (!control->refcount.compare(1)) {
      return nullptr;
    }

    T **data = control->data;
    len = control->len;

    control->data = nullptr;
    control->len = 0;
    control->alloc = 0;
    return data;
  }

  PtrArray copy() const {
    PtrArray new_array(0, control->null_terminated);

    if (control->alloc) {
      new_array.maybe_expand(control->len);

      if (control->len) {
        for (uint i = 0; i < control->len; i++) {
          new_array.control->data[i] = new T(*control->data[i]);
        }

        new_array.control->len = control->len;
      }

      new_array.maybe_null_terminate();
    }

    return new_array;
  }

  PtrArray &set_size(uint length) {
    if (length > control->len) {
      maybe_expand(length - control->len);

      for (uint i = control->len; i < length; i++) {
        control->data[i] = nullptr;
      }

      control->len = length;
      maybe_null_terminate();
    } else if (length < control->len) {
      remove_range(length, control->len - length);
    }
    return *this;
  }

  void remove_index(uint index) { remove_range(index, 1); }

  T *steal_index(uint index) {
    T *ptr = control->data[index];

    for (uint i = index; i < control->len - 1; i++) {
      control->data[i] = control->data[i + 1];
    }
    control->data[control->len - 1] = nullptr;
    control->len--;
    maybe_null_terminate();

    return ptr;
  }

  void remove_range(uint index, uint length) {
    for (uint i = index; i < index + length; i++) {
      delete control->data[i];
      control->data[i] = nullptr;
    }

    for (uint i = index + length; i < control->len; i++) {
      control->data[i - length] = control->data[i];
      control->data[i] = nullptr;
    }
    control->len -= length;
    maybe_null_terminate();
  }

  bool remove(T *ptr) {
    for (uint i = 0; i < control->len; i++) {
      if (control->data[i] == ptr) {
        remove_index(i);
        return true;
      }
    }
    return false;
  }

  void add(T *ptr) {
    maybe_expand(1);
    control->data[control->len++] = ptr;
    maybe_null_terminate();
  }

  void extend(PtrArray &other) {
    maybe_expand(other.control->len);

    for (uint i = 0; i < other.control->len; i++) {
      control->data[control->len + i] = new T(*other.control->data[i]);
    }
    control->len += other.control->len;
    maybe_null_terminate();
  }

  void extend_and_steal(PtrArray &other) {
    maybe_expand(other.control->len);

    for (uint i = 0; i < other.control->len; i++) {
      control->data[control->len + i] = other.control->data[i];
      other.control->data[i] = nullptr;
    }
    control->len += other.control->len;
    other.control->len = 0;
    maybe_null_terminate();
  }

  void insert(uint index, T *ptr) {
    maybe_expand(1);

    for (uint i = control->len; i > index; i--) {
      control->data[i] = control->data[i - 1];
    }
    control->data[index] = ptr;
    control->len++;
    maybe_null_terminate();
  }

  // TODO: Sort & find

  T *&operator[](uint index) { return control->data[index]; }
};

using ByteArray = Array<uchar>;

} // namespace GLib
