add_library("glibpp"
    "src/string.cpp"
    "src/filestream.cpp"
)
target_include_directories("glibpp" PUBLIC "include")
