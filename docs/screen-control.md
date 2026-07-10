# Screen Control

Screen control methods are thin wrappers around ANSI/VT commands for clearing, inserting, deleting, scrolling, and alternate-screen rendering. They emit output only when terminal commands are enabled.

```cpp
#include <kaycxx/term.hpp>

using namespace kaycxx::term;

int main() {
    auto term = terminal();

    term.clear_screen();
}
```

## Clearing

The clear methods erase visible terminal cells or the terminal scrollback buffer. They do not move the cursor unless the terminal itself has unusual behavior.

- `clear_screen()` clears the complete visible screen.
- `clear_screen_to_end()` clears from the cursor position to the end of the visible screen.
- `clear_screen_to_start()` clears from the beginning of the visible screen to the cursor position.
- `clear_scrollback()` clears the terminal's scrollback history when supported. This affects the off-screen history that the user can normally scroll back to, not just the currently visible cells.
- `clear_line()` clears the complete current line.
- `clear_line_to_end()` clears from the cursor position to the end of the current line.
- `clear_line_to_start()` clears from the beginning of the current line to the cursor position.

```cpp
term.clear_screen();
term.clear_screen_to_end();
term.clear_screen_to_start();
term.clear_scrollback();
term.clear_line();
term.clear_line_to_end();
term.clear_line_to_start();
```

## Lines and Cells

Line and cell operations edit terminal contents at the current cursor position. Counted variants do nothing when the count is zero or negative.

- `insert_line()` inserts one blank line at the current row. Existing lines in the active scrolling region move down.
- `insert_lines(count)` inserts multiple blank lines at the current row.
- `delete_line()` deletes one line at the current row. Lines below it move up.
- `delete_lines(count)` deletes multiple lines at the current row.
- `clear_cell()` replaces the cell at the cursor position with a blank cell. Text to the right is not shifted.
- `clear_cells(count)` replaces multiple cells starting at the cursor position with blank cells. Text to the right is not shifted.
- `insert_cell()` inserts one blank cell at the cursor position. Text to the right shifts right within the current line.
- `insert_cells(count)` inserts multiple blank cells at the cursor position.
- `delete_cell()` deletes one cell at the cursor position. Text to the right shifts left within the current line.
- `delete_cells(count)` deletes multiple cells at the cursor position.

```cpp
term.insert_line();
term.insert_lines(3);
term.delete_line();
term.delete_lines(3);

term.clear_cell();
term.clear_cells(8);
term.insert_cell();
term.insert_cells(4);
term.delete_cell();
term.delete_cells(4);
```

## Scrolling

The scroll region limits where `scroll_up` and `scroll_down` operate. Reset the region when the constrained area is no longer needed.

```cpp
term.set_scroll_region(3, 20);
term.scroll_up();
term.scroll_down(2);
term.reset_scroll_region();
```

## Alternate Screen

The alternate screen is useful for fullscreen terminal interfaces. It gives the program a separate screen buffer and usually restores the normal screen when leaving.

```cpp
term.enter_alternate_screen();
term.clear_screen();

// Render fullscreen UI.

term.leave_alternate_screen();
```
