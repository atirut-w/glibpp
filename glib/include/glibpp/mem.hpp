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

template <typename T> struct IsLvalueReference {
  static constexpr bool value = false;
};

template <typename T> struct IsLvalueReference<T &> {
  static constexpr bool value = true;
};

template <typename T>
[[nodiscard]] constexpr typename RemoveReference<T>::type &&
steal(T &&t) noexcept {
  return static_cast<typename RemoveReference<T>::type &&>(t);
}

template <typename T>
[[nodiscard]] constexpr T &&
forward(typename RemoveReference<T>::type &t) noexcept {
  return static_cast<T &&>(t);
}

template <typename T>
[[nodiscard]] constexpr T &&
forward(typename RemoveReference<T>::type &&t) noexcept {
  static_assert(
      !IsLvalueReference<T>::value,
      "GLib::forward must not be used to convert an rvalue to an lvalue");
  return static_cast<T &&>(t);
}

} // namespace GLib
