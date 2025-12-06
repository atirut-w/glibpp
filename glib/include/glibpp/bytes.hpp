#pragma once
#include "glibpp/refcount.hpp"
#include <glibpp/config.hpp>

namespace GLib {

/*
 * A simple reference counted data type representing an immutable sequence of
 * zero or more bytes from an unspecified origin.
 */
class Bytes {
  struct Control {
    const void *data = nullptr;
    gsize size = 0;
    AtomicRefCount ref_count;
  };

  Control *control = nullptr;

public:
  explicit Bytes(const void *data, gsize size);
  ~Bytes();

  Bytes(const Bytes &other);
  Bytes &operator=(const Bytes &other);
  Bytes(Bytes &&other) noexcept;
  Bytes &operator=(Bytes &&other) noexcept;
};

} // namespace GLib
