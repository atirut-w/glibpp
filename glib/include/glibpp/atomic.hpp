#pragma once

namespace GLib {

template <typename T> class Atomic;

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
  bool compare_and_exchange(int &expected, int desired) {
    return __atomic_compare_exchange_n(&value, &expected, desired, false,
                                       __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
  }
};

} // namespace GLib
