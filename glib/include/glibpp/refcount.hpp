#pragma once
#include <glibpp/atomic.hpp>

namespace GLib {

class RefCount {
  int count = 1;

public:
  void inc() { count++; }
  bool dec() { return --count == 0; }
  bool compare(int val) { return count == val; }
};

class AtomicRefCount {
  Atomic<int> count = 1;

public:
  void inc() { count.inc(); }
  bool dec() { return count.dec_and_test(); }
  bool compare(int val) { return count.get() == val; }
};

}
