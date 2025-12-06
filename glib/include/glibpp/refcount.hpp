#pragma once
#include <glibpp/atomic.hpp>

namespace GLib {

class RefCount {
  int count = 1;

public:
  RefCount() = default;

  RefCount(const RefCount &) = delete;
  RefCount &operator=(const RefCount &) = delete;

  void inc() { count++; }
  bool dec() { return --count == 0; }
  bool compare(int val) { return count == val; }
};

class AtomicRefCount {
  Atomic<int> count = 1;

public:
  AtomicRefCount() = default;

  AtomicRefCount(const AtomicRefCount &) = delete;
  AtomicRefCount &operator=(const AtomicRefCount &) = delete;

  void inc() { count++; }
  bool dec() { return count.dec_and_test(); }
  bool compare(int val) { return count == val; }
};

} // namespace GLib
