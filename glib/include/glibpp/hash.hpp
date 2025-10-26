#pragma once
#include <glibpp/config.hpp>
#include <glibpp/refcount.hpp>
#include <glibpp/types.hpp>

namespace GLib {

template <typename T> uint hash(const T &v) = delete;

template <> uint hash<const char *>(const char *const &v) {
  uint32 h = 5381;

  for (const signed char *p = reinterpret_cast<const signed char *>(v);
       *p != '\0'; p++) {
    h = ((h << 5) + h) + static_cast<uint32>(*p);
  }

  return h;
}

template <> uint hash<int>(const int &v) { return static_cast<uint>(v); }

template <> uint hash<uint>(const uint &v) { return v; }

template <> uint hash<int64>(const int64 &v) {
  return static_cast<uint>((v >> 32) ^ (v & 0xffffffffU));
}

template <> uint hash<double>(const double &v) {
  return hash<int64>(*reinterpret_cast<const int64 *>(&v));
}

template <typename K, typename V> class HashTable {
  struct Control {
    AtomicRefCount ref_count;
  };

  Control *control;

public:
  HashTable() {
    control = new Control();
  }

  HashTable(const HashTable &other) {
    control = other.control;
    control->ref_count.inc();
  }

  HashTable &operator=(const HashTable &other) {
    if (this != &other) {
      if (control->ref_count.dec()) {
        delete control;
      }
      control = other.control;
      control->ref_count.inc();
    }
    return *this;
  }

  HashTable(HashTable &&other) noexcept {
    control = other.control;
    other.control = nullptr;
  }

  HashTable &operator=(HashTable &&other) noexcept {
    if (this != &other) {
      if (control->ref_count.dec()) {
        delete control;
      }
      control = other.control;
      other.control = nullptr;
    }
    return *this;
  }

  ~HashTable() {
    if (control->ref_count.dec()) {
      delete control;
    }
  }
};

} // namespace GLib
