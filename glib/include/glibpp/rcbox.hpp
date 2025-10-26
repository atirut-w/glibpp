#pragma once
#include <glibpp/refcount.hpp>

namespace GLib {

template <typename T> class RcBox {
  struct Control {
    RefCount ref_count;
    T *data = nullptr;

    Control(T *data) : data(data) {}

    ~Control() { delete data; }
  };

  Control *control = nullptr;

  RcBox(const T &value) { control = new Control(new T(value)); }

public:
  RcBox() { control = new Control(new T()); }

  RcBox(const RcBox &other) {
    control = other.control;
    control->ref_count.inc();
  }

  RcBox &operator=(const RcBox &other) {
    if (this != &other) {
      if (control->ref_count.dec()) {
        delete control;
      }
      control = other.control;
      control->ref_count.inc();
    }
    return *this;
  }

  ~RcBox() {
    if (control->ref_count.dec()) {
      delete control;
    }
  }

  RcBox dup() const { return RcBox(*control->data); }

  T *steal() {
    T *data = control->data;
    control->data = nullptr;
    return data;
  }

  T &operator*() { return *(control->data); }

  T *operator->() { return control->data; }
};

template <typename T> class AtomicRcBox {
  struct Control {
    AtomicRefCount ref_count;
    T *data = nullptr;

    Control(T *data) : data(data) {}

    ~Control() { delete data; }
  };

  Control *control = nullptr;

  AtomicRcBox(const T &value) { control = new Control(new T(value)); }

public:
  AtomicRcBox() { control = new Control(new T()); }

  AtomicRcBox(const AtomicRcBox &other) {
    control = other.control;
    control->ref_count.inc();
  }

  AtomicRcBox &operator=(const AtomicRcBox &other) {
    if (this != &other) {
      if (control->ref_count.dec()) {
        delete control;
      }
      control = other.control;
      control->ref_count.inc();
    }
    return *this;
  }

  ~AtomicRcBox() {
    if (control->ref_count.dec()) {
      delete control;
    }
  }

  AtomicRcBox dup() const { return AtomicRcBox(*control->data); }

  T *steal() {
    T *data = control->data;
    control->data = nullptr;
    return data;
  }

  T &operator*() { return *(control->data); }

  T *operator->() { return control->data; }
};

} // namespace GLib
