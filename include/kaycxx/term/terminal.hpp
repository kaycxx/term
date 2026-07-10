// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines the terminal class.
 */

#include <ios>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>

#include <kaycxx/term/ansi_mode.hpp>
#include <kaycxx/term/basic_color.hpp>
#include <kaycxx/term/color_style.hpp>
#include <kaycxx/term/cursor_style.hpp>
#include <kaycxx/term/hyperlink.hpp>
#include <kaycxx/term/indexed_color.hpp>
#include <kaycxx/term/input_mode.hpp>
#include <kaycxx/term/rgb_color.hpp>
#include <kaycxx/term/sgr.hpp>
#include <kaycxx/term/terminal_size.hpp>

namespace kaycxx::term {

namespace detail {
class input_mode_stack;
}

/**
 * Terminal output and input helper.
 *
 * The terminal writes ANSI/VT formatting and terminal commands when enabled and provides minimal cross-platform input mode and key reading helpers.
 */
class terminal {
public:
    /**
     * Creates a terminal helper for an output stream.
     *
     * The stream defaults to `std::cout`. All text and all ANSI/VT output is written to this stream.
     * In automatic ANSI mode, the stream is used to detect terminal command support while `FORCE_COLOR` and `NO_COLOR` independently control formatting.
     *
     * @param stream  Output stream to write to. The stream must outlive the terminal.
     * @param mode    ANSI output mode. Automatic mode detects terminal commands and separately honors `FORCE_COLOR` and `NO_COLOR` for formatting.
     */
    explicit terminal(std::ostream& stream = std::cout, ansi_mode mode = ansi_mode::automatic);

    /** Destroys the terminal helper. */
    ~terminal();

    terminal(terminal const&) = delete;
    terminal& operator=(terminal const&) = delete;
    terminal(terminal&&) = delete;
    terminal& operator=(terminal&&) = delete;

    /**
     * Writes a raw stream value.
     *
     * This overload forwards the value unchanged to the wrapped stream.
     *
     * @tparam T  Value type to write.
     *
     * @param value  Value to write.
     * @returns This terminal.
     */
    template <typename T>
    terminal& operator<<(T const& value) {
        stream_ << value;
        return *this;
    }

    /**
     * Writes an SGR style command.
     *
     * @param value  SGR command to write.
     * @returns This terminal.
     */
    terminal& operator<<(sgr value);

    /**
     * Writes a foreground or background color style.
     *
     * @param value  Color style to write.
     * @returns This terminal.
     */
    terminal& operator<<(color_style value);

    /**
     * Writes a terminal hyperlink.
     *
     * @param link  Hyperlink to write.
     * @returns This terminal.
     */
    terminal& operator<<(hyperlink const& link);

    /**
     * Writes an ostream manipulator.
     *
     * @param manipulator  Manipulator to apply.
     * @returns This terminal.
     */
    terminal& operator<<(std::ostream& (*manipulator)(std::ostream&));

    /**
     * Writes an ios manipulator.
     *
     * @param manipulator  Manipulator to apply.
     * @returns This terminal.
     */
    terminal& operator<<(std::ios& (*manipulator)(std::ios&));

    /**
     * Writes an ios_base manipulator.
     *
     * @param manipulator  Manipulator to apply.
     * @returns This terminal.
     */
    terminal& operator<<(std::ios_base& (*manipulator)(std::ios_base&));

    /**
     * Clears the visible screen without moving the cursor.
     */
    void clear_screen();

    /**
     * Clears from the cursor position to the end of the screen.
     */
    void clear_screen_to_end();

    /**
     * Clears from the beginning of the screen to the cursor position.
     */
    void clear_screen_to_start();

    /**
     * Clears the terminal scrollback buffer when supported by the terminal.
     */
    void clear_scrollback();

    /**
     * Clears the current line without moving the cursor.
     */
    void clear_line();

    /**
     * Clears from the cursor position to the end of the current line.
     */
    void clear_line_to_end();

    /**
     * Clears from the beginning of the current line to the cursor position.
     */
    void clear_line_to_start();

    /**
     * Pushes the current terminal title and sets a new title.
     *
     * @param title  New terminal title.
     */
    void push_title(std::string_view title);

    /**
     * Pops the previously pushed terminal title.
     */
    void pop_title();

    /**
     * Hides the terminal cursor.
     */
    void hide_cursor();

    /**
     * Shows the terminal cursor.
     */
    void show_cursor();

    /**
     * Saves the current cursor position.
     */
    void save_cursor();

    /**
     * Restores the previously saved cursor position.
     */
    void restore_cursor();

    /**
     * Sets the terminal cursor style.
     *
     * @param style  Cursor style to apply.
     */
    void set_cursor_style(cursor_style style);

    /**
     * Moves the cursor to an absolute position.
     *
     * Coordinates are one-based terminal coordinates.
     *
     * @param column  Target column.
     * @param row     Target row.
     */
    void move_to(int column, int row);

    /**
     * Moves the cursor relative to its current position.
     *
     * Positive columns move right, negative columns move left. Positive rows move down, negative rows move up.
     *
     * @param columns  Relative column movement.
     * @param rows     Relative row movement.
     */
    void move_by(int columns, int rows);

    /**
     * Moves the cursor to a column on the current row.
     *
     * @param column  Target column.
     */
    void move_to_column(int column);

    /**
     * Enables terminal autowrap mode.
     */
    void enable_wrap();

    /**
     * Disables terminal autowrap mode.
     */
    void disable_wrap();

    /**
     * Enters the alternate screen buffer.
     */
    void enter_alternate_screen();

    /**
     * Leaves the alternate screen buffer.
     */
    void leave_alternate_screen();

    /**
     * Returns the current terminal size.
     *
     * @returns Terminal size, or zero columns and rows when the size could not be read.
     */
    terminal_size size() const;

    /**
     * Pushes the current input state and applies a new input mode.
     *
     * @param mode  Input mode to apply.
     * @returns True when the input state was saved and the requested mode was applied.
     */
    bool push_input_mode(input_mode mode);

    /**
     * Pops and restores the previously pushed input state.
     *
     * @returns True when a previous input state was restored.
     */
    bool pop_input_mode();

    /**
     * Reads one key from the terminal.
     *
     * The returned string contains the raw key byte sequence. POSIX terminals return their native byte sequences.
     * Windows console key events are mapped to POSIX-style escape sequences for cursor and function keys.
     *
     * @returns Raw key byte sequence, or an empty string on failure.
     */
    std::string read_key();

    /**
     * Inserts a single line at the cursor row.
     */
    void insert_line();

    /**
     * Inserts multiple lines at the cursor row.
     *
     * Nothing is written when the line count is zero or negative.
     *
     * @param lines  Number of lines to insert.
     */
    void insert_lines(int lines);

    /**
     * Deletes a single line at the cursor row.
     */
    void delete_line();

    /**
     * Deletes multiple lines at the cursor row.
     *
     * Nothing is written when the line count is zero or negative.
     *
     * @param lines  Number of lines to delete.
     */
    void delete_lines(int lines);

    /**
     * Clears a cell at the cursor position.
     *
     * The current cell is replaced with a blank cell. Text to the right is not shifted and the cursor stays at its position.
     */
    void clear_cell();

    /**
     * Clears cells at the cursor position.
     *
     * The cells are replaced with blank cells. Text to the right is not shifted and the cursor stays at its position.
     * Nothing is written when the cell count is zero or negative.
     *
     * @param cells  Number of cells to clear.
     */
    void clear_cells(int cells);

    /**
     * Inserts a blank cell at the cursor position.
     *
     * Existing text to the right is shifted right within the current line.
     */
    void insert_cell();

    /**
     * Inserts blank cells at the cursor position.
     *
     * Existing text to the right is shifted right within the current line. Nothing is written when the cell count is zero or negative.
     *
     * @param cells  Number of cells to insert.
     */
    void insert_cells(int cells);

    /**
     * Deletes a cell at the cursor position.
     *
     * Existing text to the right is shifted left within the current line.
     */
    void delete_cell();

    /**
     * Deletes cells at the cursor position.
     *
     * Existing text to the right is shifted left within the current line. Nothing is written when the cell count is zero or negative.
     *
     * @param cells  Number of cells to delete.
     */
    void delete_cells(int cells);

    /**
     * Sets the vertical scrolling region.
     *
     * Coordinates are one-based terminal rows. Nothing is written when the range is invalid.
     *
     * @param top     First row in the scrolling region.
     * @param bottom  Last row in the scrolling region.
     */
    void set_scroll_region(int top, int bottom);

    /**
     * Resets the vertical scrolling region to the whole screen.
     */
    void reset_scroll_region();

    /**
     * Scrolls the current scrolling region up by one line.
     */
    void scroll_up();

    /**
     * Scrolls the current scrolling region up by multiple lines.
     *
     * Nothing is written when the line count is zero or negative.
     *
     * @param lines  Number of lines to scroll.
     */
    void scroll_up(int lines);

    /**
     * Scrolls the current scrolling region down by one line.
     */
    void scroll_down();

    /**
     * Scrolls the current scrolling region down by multiple lines.
     *
     * Nothing is written when the line count is zero or negative.
     *
     * @param lines  Number of lines to scroll.
     */
    void scroll_down(int lines);

    /**
     * Checks whether ANSI/VT formatting is enabled for this terminal.
     *
     * Formatting includes colors and SGR text styles. This returns the resolved formatting state from the constructor, not the raw `ansi_mode` value.
     *
     * @returns True when ANSI/VT formatting is emitted.
     */
    bool formatting_enabled() const noexcept;

    /**
     * Checks whether ANSI/VT terminal commands are enabled for this terminal.
     *
     * Terminal commands include cursor movement, screen manipulation, title changes, hyperlinks, and terminal modes. Color environment variables do not affect this state.
     *
     * @returns True when ANSI/VT terminal commands are emitted.
     */
    bool commands_enabled() const noexcept;

private:
    /** Output stream wrapped by the terminal. */
    std::ostream& stream_;

    /** Whether ANSI/VT formatting should be written. */
    bool formatting_enabled_;

    /** Whether ANSI/VT terminal commands should be written. */
    bool commands_enabled_;

    /** Stack of saved input states. */
    std::unique_ptr<detail::input_mode_stack> input_modes_;

    /**
     * Writes a basic ANSI color.
     *
     * @param value  Basic color value.
     * @param style  Color style describing foreground or background output.
     */
    void write_color_value(basic_color value, color_style style);

    /**
     * Writes an indexed 256-color value.
     *
     * @param value  Indexed color value.
     * @param style  Color style describing foreground or background output.
     */
    void write_color_value(indexed_color value, color_style style);

    /**
     * Writes an RGB true-color value.
     *
     * @param value  RGB color value.
     * @param style  Color style describing foreground or background output.
     */
    void write_color_value(rgb_color value, color_style style);
};

} // namespace kaycxx::term
