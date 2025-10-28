#pragma once
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
        delete[] control->data;
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
        delete[] control->data;
        delete control;
      }
      control = other.control;
      other.control = nullptr;
    }
    return *this;
  }

  ~Array() {
    if (control && control->refcount.dec()) {
      delete[] control->data;
      delete control;
    }
  }
};

} // namespace GLib
