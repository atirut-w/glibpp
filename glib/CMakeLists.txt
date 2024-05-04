add_library("glibpp"
    "src/string.cpp"
    "src/file_stream.cpp"
)
target_include_directories("glibpp" PUBLIC "include")
