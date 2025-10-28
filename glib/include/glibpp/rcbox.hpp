#pragma once
#include <glibpp/refcount.hpp>

namespace GLib {

template <typename T> class RcBox {
  struct Control {
    RefCount ref_count;
    T *data = nullptr;
  };

  Control *control = nullptr;

  RcBox() = default;

public:
  template <typename... Args> static RcBox make(Args &&...args) {
    T *data = nullptr;

    try {
      // TODO: Use perfect forwarding
      data = new T(args...);
    } catch (...) {
      throw;
    }

    RcBox box;
    box.control = new Control();
    box.control->data = data;
    return box;
  }

  RcBox(const RcBox &other) : control(other.control) {
    if (control) {
      control->ref_count.inc();
    }
  }

  RcBox &operator=(const RcBox &other) {
    if (this != &other) {
      if (control && control->ref_count.dec()) {
        delete control->data;
        delete control;
      }
      control = other.control;
      if (control) {
        control->ref_count.inc();
      }
    }
    return *this;
  }

  ~RcBox() {
    if (control && control->ref_count.dec()) {
      delete control->data;
      delete control;
    }
  }

  RcBox dup() const {
    T *data = nullptr;

    try {
      data = new T(*control->data);
    } catch (...) {
      throw;
    }

    RcBox box;
    box.control = new Control();
    box.control->data = data;
    return box;
  }

  T *steal() {
    if (control && control->ref_count.get() == 1) {
      T *data = control->data;
      delete control;
      control = nullptr;
      return data;
    } else {
      return nullptr;
    }
  }

  T &operator*() { return *(control->data); }

  T *operator->() { return control->data; }
};

template <typename T> class AtomicRcBox {
  struct Control {
    AtomicRefCount ref_count;
    T *data = nullptr;
  };

  Control *control = nullptr;

  AtomicRcBox() = default;

public:
  template <typename... Args> static AtomicRcBox make(Args &&...args) {
    T *data = nullptr;

    try {
      // TODO: Use perfect forwarding
      data = new T(args...);
    } catch (...) {
      throw;
    }

    AtomicRcBox box;
    box.control = new Control();
    box.control->data = data;
    return box;
  }

  AtomicRcBox(const AtomicRcBox &other) : control(other.control) {
    if (control) {
      control->ref_count.inc();
    }
  }

  AtomicRcBox &operator=(const AtomicRcBox &other) {
    if (this != &other) {
      if (control && control->ref_count.dec()) {
        delete control->data;
        delete control;
      }
      control = other.control;
      if (control) {
        control->ref_count.inc();
      }
    }
    return *this;
  }

  ~AtomicRcBox() {
    if (control && control->ref_count.dec()) {
      delete control->data;
      delete control;
    }
  }

  AtomicRcBox dup() const {
    T *data = nullptr;

    try {
      data = new T(*control->data);
    } catch (...) {
      throw;
    }

    AtomicRcBox box;
    box.control = new Control();
    box.control->data = data;
    return box;
  }

  T *steal() {
    if (control && control->ref_count.get() == 1) {
      T *data = control->data;
      delete control;
      control = nullptr;
      return data;
    } else {
      return nullptr;
    }
  }

  T &operator*() { return *(control->data); }

  T *operator->() { return control->data; }
};

} // namespace GLib
