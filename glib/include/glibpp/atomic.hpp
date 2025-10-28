#pragma once

namespace GLib {

template <typename T> class Atomic;

template <> class Atomic<int> {
  int value;

public:
  Atomic() = default;
  Atomic(int v) : value(v) {}
  Atomic(const Atomic &other) = delete;
  Atomic &operator=(const Atomic &other) = delete;

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

  int exchange(int desired) {
    return __atomic_exchange_n(&value, desired, __ATOMIC_SEQ_CST);
  }

  int fetch_add(int v) {
    return __atomic_fetch_add(&value, v, __ATOMIC_SEQ_CST);
  }

  int fetch_and(int v) {
    return __atomic_fetch_and(&value, v, __ATOMIC_SEQ_CST);
  }

  int fetch_or(int v) { return __atomic_fetch_or(&value, v, __ATOMIC_SEQ_CST); }

  int fetch_xor(int v) {
    return __atomic_fetch_xor(&value, v, __ATOMIC_SEQ_CST);
  }

  operator int() const { return get(); }
  Atomic<int> &operator=(int v) {
    set(v);
    return *this;
  }

  int operator++() { return fetch_add(1) + 1; }
  int operator++(int) { return fetch_add(1); }
  int operator--() { return fetch_add(-1) - 1; }
  int operator--(int) { return fetch_add(-1); }

  Atomic &operator+=(int v) {
    fetch_add(v);
    return *this;
  }

  Atomic &operator-=(int v) {
    fetch_add(-v);
    return *this;
  }

  Atomic &operator&=(int v) {
    fetch_and(v);
    return *this;
  }

  Atomic &operator|=(int v) {
    fetch_or(v);
    return *this;
  }

  Atomic &operator^=(int v) {
    fetch_xor(v);
    return *this;
  }

  bool operator==(int v) const { return get() == v; }
  bool operator!=(int v) const { return get() != v; }
};

} // namespace GLib
