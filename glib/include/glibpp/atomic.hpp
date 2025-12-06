#pragma once
#include <glibpp/types.hpp>

namespace GLib {

template <typename T> class Atomic;

template <> class Atomic<int> {
  int value;

public:
  Atomic(int initial = 0) : value(initial) {}

  Atomic(const Atomic &) = delete;
  Atomic &operator=(const Atomic &) = delete;

  int get() const { return __atomic_load_n(&value, __ATOMIC_SEQ_CST); }

  void set(int newValue) {
    __atomic_store_n(&value, newValue, __ATOMIC_SEQ_CST);
  }

  int inc() { return __atomic_add_fetch(&value, 1, __ATOMIC_SEQ_CST); }

  bool dec_and_test() {
    return __atomic_sub_fetch(&value, 1, __ATOMIC_SEQ_CST) == 0;
  }

  bool compare_and_exchange(int &expected, int desired) {
    return __atomic_compare_exchange_n(&value, &expected, desired, false,
                                       __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
  }

  int exchange(int newValue) {
    return __atomic_exchange_n(&value, newValue, __ATOMIC_SEQ_CST);
  }

  int fetch_add(int arg) {
    return __atomic_fetch_add(&value, arg, __ATOMIC_SEQ_CST);
  }

  uint fetch_and(uint arg) {
    return __atomic_fetch_and(reinterpret_cast<uint *>(&value), arg,
                              __ATOMIC_SEQ_CST);
  }

  uint fetch_or(uint arg) {
    return __atomic_fetch_or(reinterpret_cast<uint *>(&value), arg,
                             __ATOMIC_SEQ_CST);
  }

  uint fetch_xor(uint arg) {
    return __atomic_fetch_xor(reinterpret_cast<uint *>(&value), arg,
                              __ATOMIC_SEQ_CST);
  }

  operator int() const { return get(); }
  int operator=(int newValue) {
    set(newValue);
    return newValue;
  }

  int operator++() { return inc(); }
  int operator++(int) { return fetch_add(1); }
  int operator--() { return __atomic_sub_fetch(&value, 1, __ATOMIC_SEQ_CST); }
  int operator--(int) {
    return __atomic_fetch_sub(&value, 1, __ATOMIC_SEQ_CST);
  }

  int operator+=(int rhs) { return fetch_add(rhs) + rhs; }
  int operator-=(int rhs) { return fetch_add(-rhs) - rhs; }
  int operator&=(int rhs) {
    return fetch_and(static_cast<uint>(rhs)) & rhs;
  }
  int operator|=(int rhs) {
    return fetch_or(static_cast<uint>(rhs)) | rhs;
  }
  int operator^=(int rhs) {
    return fetch_xor(static_cast<uint>(rhs)) ^ rhs;
  }
};

} // namespace GLib
