#pragma once

namespace GLib {

template <typename T> class Atomic {};

template <> class Atomic<int> {
  int value;

public:
  Atomic() : value(0) {}
  Atomic(int v) : value(v) {}
};

template <typename T> class Atomic<T *> {
  T *value;

public:
  Atomic() : value(nullptr) {}
  Atomic(T *v) : value(v) {}
};

} // namespace GLib
