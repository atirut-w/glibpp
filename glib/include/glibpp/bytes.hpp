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

  const void *get_data() const {
    return control ? control->data : nullptr;
  }

  gsize get_size() const {
    return control ? control->size : 0;
  }

  // TODO: Unref to raw data & ByteArray
  // TODO: g_bytes_get_region
};

} // namespace GLib
