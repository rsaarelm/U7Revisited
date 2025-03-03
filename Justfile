meson_setup_args := if os_family() == "windows" { "--backend=vs" } else { "" }

run: build
    cd Redist; ../build/U7Revisited

build: setup
    meson compile -C build

setup:
    meson setup --buildtype=release {{ meson_setup_args }} build

run-debug: build-debug
    cd Redist; ../build-dbg/U7Revisited

build-debug: setup-debug
    meson compile -C build-dbg

setup-debug:
    meson setup {{ meson_setup_args }} build-dbg

# Check project sources for style problems.
lint:
    find Source/ -name '*.cpp' -o -name '*.h' | xargs cpplint

# Format all project source code with ClangFormat.
format-sources:
    find Source/ -name '*.cpp' -o -name '*.h' | xargs clang-format -i --verbose

# NB. clang-format doesn't seem to always do all the work it can in one run,
# particularly if formatting a codebase for the first time. You may want to run
# it twice and see if it does any extra work the second time. Once the next
# clang-format run does nothing, you're good.
