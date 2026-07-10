# Cursor Control

Cursor control methods write ANSI/VT cursor sequences when terminal commands are enabled. Coordinates are one-based terminal coordinates: column `1`, row `1` is the top-left cell.

```cpp
#include <kaycxx/term.hpp>

using namespace kaycxx::term;

int main() {
    auto term = terminal();

    term.move_to(10, 4);
    term << "x";
}
```

## Movement

Use `move_to` for absolute movement, `move_by` for relative movement, and `move_to_column` when only the column should change.

```cpp
term.move_to(1, 1);
term.move_by(4, 0);
term.move_by(0, 2);
term.move_to_column(1);
```

`move_by(columns, rows)` uses positive columns for right, negative columns for left, positive rows for down, and negative rows for up.

## Visibility

The cursor can be hidden while rendering and shown again afterwards.

```cpp
term.hide_cursor();

// Render UI.

term.show_cursor();
```

## Style

`set_cursor_style` changes the shape and blinking behavior of the cursor when the terminal supports it.

```cpp
term.set_cursor_style(cursor_style::steady_bar);
```

Available cursor styles:

- `cursor_style::standard`: Terminal-defined standard cursor style.
- `cursor_style::blinking_block`: Blinking block cursor.
- `cursor_style::steady_block`: Steady block cursor.
- `cursor_style::blinking_underline`: Blinking underline cursor.
- `cursor_style::steady_underline`: Steady underline cursor.
- `cursor_style::blinking_bar`: Blinking vertical bar cursor.
- `cursor_style::steady_bar`: Steady vertical bar cursor.

## Save and Restore

`save_cursor` stores the current cursor position in the terminal, and `restore_cursor` moves back to it.

```cpp
term.save_cursor();
term.move_to(1, 1);
term << "header";
term.restore_cursor();
```
