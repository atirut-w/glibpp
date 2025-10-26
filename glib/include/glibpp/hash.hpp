#pragma once
#include <glibpp/config.hpp>
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

} // namespace GLib
