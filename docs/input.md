# Input

`kaycxx-term` provides minimal key input helpers. `terminal::read_key` returns the raw key byte sequence as a `std::string`.

```cpp
#include <kaycxx/term.hpp>

using namespace kaycxx::term;

int main() {
    auto term = terminal();

    auto key = term.read_key();
    term << "Read " << key.size() << " byte(s)\n";
}
```

## Raw Mode

`read_key` automatically switches to raw input mode when raw mode is not already active. When `read_key` enables raw mode itself, it restores the previous input state after the key has been read.

For manual control, use `push_input_mode` and `pop_input_mode`:

```cpp
if (term.push_input_mode(input_mode::raw)) {
    auto first = term.read_key();
    auto second = term.read_key();

    term.pop_input_mode();
}
```

The input mode stack stores native terminal state snapshots, so nested pushes restore in last-in-first-out order.

## Key Sequences

On POSIX terminals, `read_key` returns the terminal's native byte sequence. Normal UTF-8 text input is returned as its UTF-8 bytes. Special keys such as cursor keys and function keys usually return escape sequences.

On Windows, console key events are mapped to POSIX-style escape sequences for cursor and function keys. Normal Unicode input is encoded as UTF-8 where possible.

```cpp
auto key = term.read_key();

if (key == "\033") {
    term << "Escape\n";
} else if (key == "\033[A") {
    term << "Cursor up\n";
}
```

This is intentionally low-level. Higher-level key event parsing can be built on top without hiding what the terminal actually produced.
