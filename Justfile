help:
    just --list

# Format all project source code with ClangFormat
format-source:
    find Source/ -name '*.cpp' -o -name '*.h' | xargs clang-format -i --verbose
