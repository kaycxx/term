# kaycxx-term

Terminal output and key input helpers.

[GitHub] | [API Documentation]

## Requirements

- C++23 compiler and standard library
- Exception support

## Usage

```cpp
#include <kaycxx/term.hpp>

using namespace kaycxx::term;

int main() {
    auto term = terminal();

    term << fg(green)
         << "Ready"
         << reset
         << '\n';
}
```

CMake users consume the installed package with:

```cmake
find_package(kaycxx-term 1.0.0 CONFIG REQUIRED)
target_link_libraries(my-target PRIVATE kaycxx::term)
```

Non-CMake users can use pkg-config:

```sh
c++ $(pkg-config --cflags kaycxx-term) -c main.cpp
c++ main.o $(pkg-config --libs kaycxx-term)
```

## Guides

- [Creating a Terminal] explains output streams, ANSI mode selection, and color environment variables.
- [Colors and Styles] explains SGR styles, basic colors, indexed colors, RGB colors, foregrounds, and backgrounds.
- [Cursor Control] explains cursor movement, cursor visibility, cursor style, and save/restore.
- [Screen Control] explains clearing, line/cell insertion and deletion, scrolling, and the alternate screen.
- [Links and Titles] explains terminal hyperlinks and window title push/pop.
- [Input] explains raw mode, input mode stacking, key reading, and Windows key sequence mapping.

## Build From Source

```sh
cmake -B build
cmake --build build
```

A shared library is built by default. For a static build:

```sh
cmake -B build -D BUILD_SHARED_LIBS=OFF
cmake --build build
```

## Install

```sh
cmake --install build --prefix /tmp/root
```

If no prefix is specified, CMake installs to `/usr/local` by default on Unix systems.

## Development

Run all tests:

```sh
cmake --build build --target test
```

Generate API documentation with Doxygen:

```sh
cmake --build build --target apidoc
```

The generated HTML documentation is written to `build/apidoc/html/index.html`.

[GitHub]: https://github.com/kaycxx/term
[API Documentation]: https://kaycxx.github.io/term/
[Creating a Terminal]: docs/creating-a-terminal.md
[Colors and Styles]: docs/colors-and-styles.md
[Cursor Control]: docs/cursor-control.md
[Screen Control]: docs/screen-control.md
[Links and Titles]: docs/links-and-titles.md
[Input]: docs/input.md
