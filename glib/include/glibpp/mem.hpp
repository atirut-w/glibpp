// TODO: Organize the utility classes!
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

template <typename T> class AutoPtr {
  T *ptr = nullptr;

  AutoPtr() = default;

public:
  template <typename... Args> static AutoPtr make(Args &&...args) {
    AutoPtr ap;
    ap.ptr = new T(forward<Args>(args)...);
    return ap;
  }

  AutoPtr(const AutoPtr &other) = delete;
  AutoPtr &operator=(const AutoPtr &other) = delete;

  AutoPtr(AutoPtr &&other) noexcept : ptr(other.ptr) {
    other.ptr = nullptr;
  }

  AutoPtr &operator=(AutoPtr &&other) noexcept {
    if (this != &other) {
      delete ptr;
      ptr = other.ptr;
      other.ptr = nullptr;
    }
    return *this;
  }

  ~AutoPtr() {
    delete ptr;
  }

  T &operator*() const {
    return *ptr;
  }

  T *operator->() const {
    return ptr;
  }
};

} // namespace GLib
