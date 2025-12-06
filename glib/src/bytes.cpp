#include <cstring>
#include <glibpp/bytes.hpp>

namespace GLib {

Bytes::Bytes(const void *data, gsize size) {
  control = new Control;
  control->data = new char[size];
  std::memcpy(const_cast<void *>(control->data), data, size);
  control->size = size;
}

Bytes::~Bytes() {
  if (control && control->ref_count.dec()) {
    delete static_cast<const char *>(control->data);
    delete control;
  }
}

Bytes::Bytes(const Bytes &other) : control(other.control) {
  if (control) {
    control->ref_count.inc();
  }
}

Bytes &Bytes::operator=(const Bytes &other) {
  if (this != &other) {
    if (control && control->ref_count.dec()) {
      delete static_cast<const char *>(control->data);
      delete control;
    }
    control = other.control;
    if (control) {
      control->ref_count.inc();
    }
  }
  return *this;
}

Bytes::Bytes(Bytes &&other) noexcept : control(other.control) {
  other.control = nullptr;
}

Bytes &Bytes::operator=(Bytes &&other) noexcept {
  if (this != &other) {
    if (control && control->ref_count.dec()) {
      delete static_cast<const char *>(control->data);
      delete control;
    }
    control = other.control;
    other.control = nullptr;
  }
  return *this;
}

} // namespace GLib
