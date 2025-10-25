#pragma once
#include <glibpp/refcount.hpp>
#include <string.h>

namespace GLib {

template <typename T> class RcBox {
  struct Control {
    RefCount ref_count;
    T *data = nullptr;
  };

  Control *control = nullptr;

public:
  RcBox() {
    control = new Control();
    control->data = new T();
  }

  RcBox(const RcBox &other) {
    control = other.control;
    control->ref_count.inc();
  }

  RcBox &operator=(const RcBox &other) {
    if (this != &other) {
      if (control->ref_count.dec()) {
        delete control->data;
        delete control;
      }
      control = other.control;
      control->ref_count.inc();
    }
    return *this;
  }

  ~RcBox() {
    if (control->ref_count.dec()) {
      delete control->data;
      delete control;
    }
  }

  RcBox dup() const {
    RcBox copy;
    delete copy.control->data;
    copy.control->data = new T(*control->data);
    return copy;
  }

  T *steal() {
    T *data = control->data;
    control->data = nullptr;
    return data;
  }

  T &operator*() {
    return *(control->data);
  }

  T *operator->() {
    return control->data;
  }
};

template <typename T> class AtomicRcBox {
  struct Control {
    AtomicRefCount ref_count;
    T *data = nullptr;
  };

  Control *control = nullptr;

public:
  AtomicRcBox() {
    control = new Control();
    control->data = new T();
  }

  AtomicRcBox(const AtomicRcBox &other) {
    control = other.control;
    control->ref_count.inc();
  }

  AtomicRcBox &operator=(const AtomicRcBox &other) {
    if (this != &other) {
      if (control->ref_count.dec()) {
        delete control->data;
        delete control;
      }
      control = other.control;
      control->ref_count.inc();
    }
    return *this;
  }

  ~AtomicRcBox() {
    if (control->ref_count.dec()) {
      delete control->data;
      delete control;
    }
  }

  AtomicRcBox dup() const {
    AtomicRcBox copy;
    delete copy.control->data;
    copy.control->data = new T(*control->data);
    return copy;
  }

  T *steal() {
    T *data = control->data;
    control->data = nullptr;
    if (control->ref_count.dec()) {
      delete control;
    }
    control = nullptr;
    return data;
  }

  T &operator*() {
    return *(control->data);
  }

  T *operator->() {
    return control->data;
  }
};

} // namespace GLib
