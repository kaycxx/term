# Creating a Terminal

A `terminal` wraps an output stream and decides whether ANSI/VT control sequences should be written to that stream.

```cpp
#include <kaycxx/term.hpp>

using namespace kaycxx::term;

int main() {
    auto term = terminal();

    term << "Hello\n";
}
```

The default constructor writes to `std::cout` and uses `ansi_mode::automatic`.

## Output Stream

The first constructor parameter is the output stream. All normal text and all control sequences are written to this stream.

```cpp
#include <iostream>

#include <kaycxx/term.hpp>

using namespace kaycxx::term;

int main() {
    auto output = terminal(std::cout);
    auto error = terminal(std::cerr);

    output << "normal output\n";
    error << fg(red) << "error output" << reset << '\n';
}
```

The terminal stores the stream by reference, so the stream must outlive the terminal object.

Automatic ANSI detection is only meaningful for known terminal streams. `std::cout` maps to stdout. `std::cerr` and `std::clog` map to stderr. Other streams are treated as not ANSI-capable unless ANSI output is forced.

## ANSI Mode

The second constructor parameter controls when ANSI/VT control sequences are emitted.

```cpp
auto automatic = terminal(std::cout, ansi_mode::automatic);
auto forced = terminal(std::cout, ansi_mode::always);
auto plain = terminal(std::cout, ansi_mode::never);
```

`ansi_mode::automatic` is the default. It first checks color-related environment variables and then falls back to terminal support detection.

`ansi_mode::always` always writes ANSI/VT control sequences. Use it when the output stream is known to understand ANSI even though automatic detection cannot prove it.

`ansi_mode::never` never writes ANSI/VT control sequences. Normal text is still written, but colors, cursor movement, clearing, hyperlinks, and other control operations do not emit escape sequences.

## Environment Variables

Automatic mode honors two common environment variables:

- `FORCE_COLOR`: When set to a non-empty value, ANSI/VT output is enabled.
- `NO_COLOR`: When set to a non-empty value, ANSI/VT output is disabled.

`FORCE_COLOR` has priority over `NO_COLOR` in this library. If both are set and non-empty, ANSI/VT output is enabled.

When neither variable is set, automatic mode checks whether the wrapped stream is connected to a terminal. On Windows, it also tries to enable virtual terminal output processing for stdout or stderr.
