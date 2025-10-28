#pragma once

namespace GLib {

template <typename T> struct RemoveReference {
  using type = T;
};

template <typename T> struct RemoveReference<T &> {
  using type = T;
};

template <typename T> struct RemoveReference<T &&> {
  using type = T;
};

template <typename T>
[[nodiscard]] constexpr typename RemoveReference<T>::type &&
steal(T &&t) noexcept {
  return static_cast<typename RemoveReference<T>::type &&>(t);
}

} // namespace GLib
