#pragma once

namespace GLib {

template <typename T> class Atomic {
  T value;

public:
  Atomic() : value() {}
  Atomic(T v) : value(v) {}

  T get() const { return __atomic_load_n(&value, __ATOMIC_SEQ_CST); }
  void set(T v) { __atomic_store_n(&value, v, __ATOMIC_SEQ_CST); }
};

template <> class Atomic<int> {
  int value;

public:
  Atomic() : value(0) {}
  Atomic(int v) : value(v) {}

  int get() const { return __atomic_load_n(&value, __ATOMIC_SEQ_CST); }
  void set(int v) { __atomic_store_n(&value, v, __ATOMIC_SEQ_CST); }
  void inc() { __atomic_fetch_add(&value, 1, __ATOMIC_SEQ_CST); }
  bool dec_and_test() {
    return __atomic_sub_fetch(&value, 1, __ATOMIC_SEQ_CST) == 0;
  }
};

template <typename T> class Atomic<T *> {
  T *value;

public:
  Atomic() : value(nullptr) {}
  Atomic(T *v) : value(v) {}
};

} // namespace GLib
