add_library("glibpp"
    "src/string.cpp"
    "src/glib/file_stream.cpp"
    "src/glib/io.cpp"
)
target_include_directories("glibpp" PUBLIC "include")
