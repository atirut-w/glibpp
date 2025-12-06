#pragma once

namespace GLib {

template <typename T> class Atomic;

template <> class Atomic<int> {};

template <typename T> class Atomic<T *> {};

} // namespace GLib
