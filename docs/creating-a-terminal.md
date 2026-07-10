# Creating a Terminal

A `terminal` wraps an output stream and independently decides whether ANSI/VT formatting and terminal commands should be written to that stream.

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

The first constructor parameter is the output stream. All normal text, formatting, and terminal commands are written to this stream.

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

Automatic terminal detection is only meaningful for known terminal streams. `std::cout` maps to stdout. `std::cerr` and `std::clog` map to stderr. Other streams are treated as not terminal-capable unless ANSI output is forced.

## ANSI Mode

The second constructor parameter controls when ANSI/VT output is emitted.

```cpp
auto automatic = terminal(std::cout, ansi_mode::automatic);
auto plain = terminal(std::cout, ansi_mode::none);
auto formatted = terminal(std::cout, ansi_mode::formatting_only);
auto commands = terminal(std::cout, ansi_mode::commands_only);
auto full = terminal(std::cout, ansi_mode::full);
```

`ansi_mode::automatic` is the default. Terminal commands are enabled only when the wrapped stream is connected to a supported terminal. Formatting independently honors the color-related environment variables and otherwise uses the detected terminal support.

`ansi_mode::none` never writes ANSI/VT output. Normal text is still written, but colors, text styles, cursor movement, clearing, hyperlinks, and other terminal commands do not emit escape sequences.

`ansi_mode::formatting_only` writes colors and text styles but no terminal commands. This is suitable for line-oriented logs that understand SGR formatting but do not provide an interactive terminal.

`ansi_mode::commands_only` writes terminal commands but no colors or text styles.

`ansi_mode::full` always writes ANSI/VT formatting and terminal commands. Use it when the output stream is known to understand ANSI even though automatic detection cannot prove it.

## Environment Variables

Automatic mode honors two common environment variables for colors and text styles:

- `FORCE_COLOR`: When set to a non-empty value, ANSI/VT colors and text styles are enabled.
- `NO_COLOR`: When set to a non-empty value, ANSI/VT colors and text styles are disabled.

`FORCE_COLOR` has priority over `NO_COLOR` in this library. If both are set and non-empty, colors and text styles are enabled.

These variables never enable or disable terminal commands such as cursor movement or screen clearing. This allows colored output in line-oriented CI logs without emitting commands that require an interactive terminal.

When neither variable is set, colors and text styles follow the detected terminal support. On Windows, detection also tries to enable virtual terminal output processing for stdout or stderr.

## Resolved Output State

Use `formatting_enabled()` and `commands_enabled()` to inspect the two independently resolved output states.

```cpp
auto term = terminal();

if (term.formatting_enabled()) {
    term << "ANSI/VT colors and text styles are active\n";
}

if (term.commands_enabled()) {
    term << "ANSI/VT terminal commands are active\n";
}
```
