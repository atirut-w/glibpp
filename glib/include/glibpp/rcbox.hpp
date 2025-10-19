#pragma once
#include <glibpp/refcount.hpp>
#include <string.h>

namespace GLib {

template <typename T> class RcBox {
  RefCount *ref_count = nullptr;
  T *data = nullptr;

public:
  RcBox() = default;

  RcBox(const T &value) {
    ref_count = new RefCount();
    data = new T(value);
  }

  RcBox(const RcBox &other) {
    ref_count = other.ref_count;
    data = other.data;
    if (ref_count) {
      ref_count->inc();
    }
  }

  RcBox &operator=(const RcBox &other) {
    if (this != &other) {
      if (ref_count && ref_count->dec()) {
        delete data;
        delete ref_count;
      }
      ref_count = other.ref_count;
      data = other.data;
      if (ref_count) {
        ref_count->inc();
      }
    }
    return *this;
  }

  ~RcBox() {
    if (ref_count && ref_count->dec()) {
      delete data;
      delete ref_count;
    }
  }

  RcBox dup() const {
    RcBox copy;
    copy.ref_count = new RefCount();
    copy.data = new T(*data);
    return copy;
  }

  T &operator*() {
    return *data;
  }

  T *operator->() {
    return data;
  }
};

template <typename T> class AtomicRcBox {
  AtomicRefCount *ref_count = nullptr;
  T *data = nullptr;

public:
  AtomicRcBox() = default;

  AtomicRcBox(const T &value) {
    ref_count = new AtomicRefCount();
    data = new T(value);
  }

  AtomicRcBox(const AtomicRcBox &other) {
    ref_count = other.ref_count;
    data = other.data;
    if (ref_count) {
      ref_count->inc();
    }
  }

  AtomicRcBox &operator=(const AtomicRcBox &other) {
    if (this != &other) {
      if (ref_count && ref_count->dec()) {
        delete data;
        delete ref_count;
      }
      ref_count = other.ref_count;
      data = other.data;
      if (ref_count) {
        ref_count->inc();
      }
    }
    return *this;
  }

  ~AtomicRcBox() {
    if (ref_count && ref_count->dec()) {
      delete data;
      delete ref_count;
    }
  }

  AtomicRcBox dup() const {
    AtomicRcBox copy;
    copy.ref_count = new AtomicRefCount();
    copy.data = new T(*data);
    return copy;
  }

  T &operator*() {
    return *data;
  }

  T *operator->() {
    return data;
  }
};

}
