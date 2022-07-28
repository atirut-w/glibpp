add_library(glibpp src/glibpp.cpp)
target_include_directories(glibpp PUBLIC src/include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)

target_link_libraries(glibpp ${GLIB_LIBRARIES})
target_include_directories(glibpp PUBLIC ${GLIB_INCLUDE_DIRS})
