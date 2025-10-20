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
  RcBox() = default;

  RcBox(const T &value) {
    control = new Control();
    control->data = new T(value);
  }

  RcBox(const RcBox &other) {
    control = other.control;
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
    RcBox copy;
    copy.control = new Control();
    if (control && control->data) {
      copy.control->data = new T(*control->data);
    }
    return copy;
  }

  T *steal() {
    if (!control) {
      return nullptr;
    }
    T *data = control->data;
    control->data = nullptr;
    if (control->ref_count.dec()) {
      delete control;
    }
    control = nullptr;
    return data;
  }

  operator T*() {
    if (control) {
      return control->data;
    }
    return nullptr;
  }
};

template <typename T> class AtomicRcBox {
  struct Control {
    AtomicRefCount ref_count;
    T *data = nullptr;
  };

  Control *control = nullptr;

public:
  AtomicRcBox() = default;

  AtomicRcBox(const T &value) {
    control = new Control();
    control->data = new T(value);
  }

  AtomicRcBox(const AtomicRcBox &other) {
    control = other.control;
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
    AtomicRcBox copy;
    copy.control = new Control();
    if (control && control->data) {
      copy.control->data = new T(*control->data);
    }
    return copy;
  }

  T *steal() {
    if (!control) {
      return nullptr;
    }
    T *data = control->data;
    control->data = nullptr;
    if (control->ref_count.dec()) {
      delete control;
    }
    control = nullptr;
    return data;
  }

  operator T*() {
    if (control) {
      return control->data;
    }
    return nullptr;
  }
};

}
