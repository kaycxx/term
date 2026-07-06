# Colors and Styles

`kaycxx-term` writes Select Graphic Rendition (SGR) sequences through stream values. ANSI/VT output is controlled when the terminal is created.

```cpp
#include <kaycxx/term.hpp>

using namespace kaycxx::term;

int main() {
    auto term = terminal();

    term << bold << fg(green) << "success" << reset << '\n';
    term << fg(bright_red) << "error" << reset_foreground << '\n';
    term << bg(rgb(20, 40, 80)) << fg(bright_white) << "status" << reset << '\n';
}
```

Use `reset` to clear all SGR attributes. Use `reset_foreground` and `reset_background` when only one color channel should be restored.

## Basic Colors

Basic colors use the ANSI 16-color palette. The concrete rendered color depends on the terminal theme.

```cpp
term << fg(red) << "red" << reset_foreground << '\n';
term << fg(bright_blue) << "bright blue" << reset_foreground << '\n';
term << bg(black) << fg(white) << "themed contrast" << reset << '\n';
```

## Indexed Colors

Indexed colors use the terminal's 256-color palette. Indices `0` to `15` normally match the basic palette, `16` to `231` are a color cube, and `232` to `255` are grayscale values.

```cpp
term << fg(indexed(196)) << "palette red" << reset_foreground << '\n';
```

## RGB Colors

RGB colors use true-color SGR sequences. Terminals that do not support true color may approximate or ignore them.

```cpp
term << fg(rgb(255, 128, 0)) << "orange" << reset_foreground << '\n';
```

## Styles

The library exposes common SGR attributes directly:

```cpp
term << bold << "bold" << normal_intensity << '\n';
term << italic << "italic" << no_italic << '\n';
term << underline << "underlined" << no_underline << '\n';
term << inverse << "inverse" << no_inverse << '\n';
```
