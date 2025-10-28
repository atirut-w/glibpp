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
    RcBox box;
    box.control = new Control();

    try {
      // TODO: Use perfect forwarding
      box.control->data = new T(args...);
    } catch (...) {
      delete box.control;
      throw;
    }

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
    RcBox box;
    box.control = new Control();

    try {
      box.control->data = new T(*control->data);
    } catch (...) {
      delete box.control;
      throw;
    }

    return box;
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
    AtomicRcBox box;
    box.control = new Control();

    try {
      // TODO: Use perfect forwarding
      box.control->data = new T(args...);
    } catch (...) {
      delete box.control;
      throw;
    }

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
    AtomicRcBox box;
    box.control = new Control();

    try {
      box.control->data = new T(*control->data);
    } catch (...) {
      delete box.control;
      throw;
    }

    return box;
  }

  T &operator*() { return *(control->data); }

  T *operator->() { return control->data; }
};

} // namespace GLib
