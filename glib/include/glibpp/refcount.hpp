#pragma once
#include <glibpp/atomic.hpp>

namespace GLib {

class RefCount {
  int count = 1;

public:
  int get() const { return count; }
  void inc() { count++; }
  bool dec() { return --count == 0; }
  bool compare(int val) { return count == val; }
};

class AtomicRefCount {
  Atomic<int> count = 1;

public:
  int get() const { return count; }
  void inc() { count++; }
  bool dec() { return --count == 0; }
  bool compare(int val) { return count == val; }
};

} // namespace GLib
