#pragma once
#include <glibpp/refcount.hpp>
#include <glibpp/types.hpp>

namespace GLib {

template <typename T> class Array {
  struct Control {
    T *data = nullptr;
    uint len = 0;
    uint capacity = 0;
    AtomicRefCount ref_count;
  };

  Control *control = nullptr;

  void maybe_expand(uint len) {
    if (control->len + len > control->capacity) {
      uint new_capacity = 1;
      while (new_capacity < control->len + len) {
        new_capacity *= 2;
      }

      T *new_data = new T[new_capacity];
      for (uint i = 0; i < control->len; ++i) {
        new_data[i] = control->data[i];
      }
      delete[] control->data;
      control->data = new_data;
      control->capacity = new_capacity;
    }
  }

public:
  Array() { control = new Control(); }

  Array(const Array &other) {
    control = other.control;
    control->ref_count.inc();
  }

  ~Array() {
    if (control->ref_count.dec()) {
      delete[] control->data;
      delete control;
    }
  }

  T *data() { return control->data; }

  uint len() const { return control->len; }

  Array &append_val(const T &value) { return append_vals(&value, 1); }

  Array &append_vals(const T *data, uint len) {
    if (len == 0) {
      return *this;
    }

    maybe_expand(len);

    for (uint i = 0; i < len; ++i) {
      control->data[control->len + i] = data[i];
    }
    control->len += len;

    return *this;
  }

  Array &prepend_val(const T &value) { return prepend_vals(&value, 1); }

  Array &prepend_vals(const T *data, uint len) {
    if (len == 0) {
      return *this;
    }

    maybe_expand(len);

    for (uint i = control->len; i > 0; --i) {
      control->data[i + len - 1] = control->data[i - 1];
    }
    for (uint i = 0; i < len; ++i) {
      control->data[i] = data[i];
    }
    control->len += len;

    return *this;
  }

  Array &insert_val(uint index, const T &value) {
    return insert_vals(index, &value, 1);
  }

  Array &insert_vals(uint index, const T *data, uint len) {
    if (len == 0 || index > control->len) {
      return *this;
    }

    maybe_expand(len);

    for (uint i = control->len; i > index; --i) {
      control->data[i + len - 1] = control->data[i - 1];
    }
    for (uint i = 0; i < len; ++i) {
      control->data[index + i] = data[i];
    }
    control->len += len;

    return *this;
  }

  T &index(uint i) { return control->data[i]; }

  Array &set_size(uint length) {
    if (length > control->len) {
      maybe_expand(length - control->len);
    } else if (length < control->len) {
      for (uint i = length; i < control->len; ++i) {
        control->data[i].~T();
      }
    }

    control->len = length;
    return *this;
  }

  Array &remove_index(uint index) {
    if (index != control->len - 1) {
      for (uint i = index; i < control->len - 1; ++i) {
        control->data[i] = control->data[i + 1];
      }
    }
    control->len--;
  }

  Array &remove_range(uint index, uint length) {
    if (length == 0) {
      return *this;
    }

    if (index + length != control->len) {
      for (uint i = index; i < control->len - length; ++i) {
        control->data[i] = control->data[i + length];
      }
    }
    control->len -= length;

    return *this;
  }
};

template <typename T> class PtrArray {
  struct Control {
    T **pdata = nullptr;
    uint len = 0;
    uint alloc = 0;
    AtomicRefCount ref_count;
  };

  Control *control = nullptr;

  void maybe_expand(uint len) {
    if (control->len + len > control->alloc) {
      uint new_alloc = 1;
      while (new_alloc < control->len + len) {
        new_alloc *= 2;
      }

      T **new_data = new T*[new_alloc];
      for (uint i = 0; i < control->len; ++i) {
        new_data[i] = control->pdata[i];
      }
      delete[] control->pdata;
      control->pdata = new_data;
      control->alloc = new_alloc;
    }
  }

  T *remove_index(uint index, bool fast, bool free_element) {
    T *result = control->pdata[index];

    if (free_element) {
      delete result;
    }

    if (index != control->len - 1 && !fast) {
      for (uint i = index; i < control->len - 1; ++i) {
        control->pdata[i] = control->pdata[i + 1];
      }
    } else if (index != control->len - 1) {
      control->pdata[index] = control->pdata[control->len - 1];
    }
    control->len--;

    return result;
  }

public:
  PtrArray() { control = new Control(); }

  PtrArray(const PtrArray &other) {
    control = other.control;
    control->ref_count.inc();
  }

  ~PtrArray() {
    if (control->ref_count.dec()) {
      delete[] control->pdata;
      delete control;
    }
  }

  T **data() { return control->pdata; }

  uint len() const { return control->len; }

  void set_size(uint length) {
    if (length > control->len) {
      maybe_expand(length - control->len);
    }

    for (uint i = control->len; i < length; ++i) {
      control->pdata[i] = nullptr;
    }
    control->len = length;
  }

  T *remove_index(uint index) {
    return remove_index(index, false, true);
  }

  T *remove_index_fast(uint index) {
    return remove_index(index, true, true);
  }

  T *steal_index(uint index) {
    return remove_index(index, false, false);
  }

  T *steal_index_fast(uint index) {
    return remove_index(index, true, false);
  }

  PtrArray &remove_range(uint index, uint length) {
    if (length == 0) {
      return *this;
    }

    for (uint i = index; i < index + length; ++i) {
      delete control->pdata[i];
    }

    if (index + length != control->len) {
      for (uint i = index; i < control->len - length; ++i) {
        control->pdata[i] = control->pdata[i + length];
      }
    }

    control->len -= length;
    return *this;
  }

  bool remove(T *data) {
    for (uint i = 0; i < control->len; ++i) {
      if (control->pdata[i] == data) {
        remove_index(i);
        return true;
      }
    }
    return false;
  }

  bool remove_fast(T *data) {
    for (uint i = 0; i < control->len; ++i) {
      if (control->pdata[i] == data) {
        remove_index_fast(i);
        return true;
      }
    }
    return false;
  }

  void add(T *data) {
    maybe_expand(1);
    control->pdata[control->len++] = data;
    return *this;
  }

  void extend(PtrArray &other) {
    if (other.len() == 0) {
      return;
    }

    maybe_expand(other.len());

    for (uint i = 0; i < other.len(); ++i) {
      control->pdata[control->len + i] = new T(*(other.data()[i]));
    }

    control->len += other.len();
    return *this;
  }

  void extend_and_steal(PtrArray &other) {
    extend(other);
    T **pdata = other.data();
    other.control->pdata = nullptr;
    other.control->len = 0;
    other.control->alloc = 0;
    other.~PtrArray();
    delete pdata;
  }

  void insert(uint index, T *data) {
    maybe_expand(1);

    uint real_index = (index >= 0) ? index : control->len;
    if (real_index < control->len) {
      for (uint i = control->len; i > real_index; --i) {
        control->pdata[i] = control->pdata[i - 1];
      }
    }
    control->len++;
    control->pdata[real_index] = data;
  }
};

} // namespace GLib
