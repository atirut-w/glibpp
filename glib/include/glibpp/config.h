#pragma once

namespace GLib {

// TODO: Make these types configurable via CMake

using int8 = signed char;
using uint8 = unsigned char;

using int16 = signed short;
using uint16 = unsigned short;

using int32 = signed int;
using uint32 = unsigned int;

using int64 = signed long;
using uint64 = unsigned long;

using ssize = signed long;
using size = unsigned long;

using offset = int64;

using intptr = signed long;
using uintptr = unsigned long;

}
