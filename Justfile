# Check project sources for style problems.
cpplint:
    find Source/ -name '*.cpp' -o -name '*.h' | xargs cpplint

# Format all project source code with ClangFormat.
format-source:
    find Source/ -name '*.cpp' -o -name '*.h' | xargs clang-format -i --verbose
