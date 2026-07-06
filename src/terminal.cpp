// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include <kaycxx/term/terminal.hpp>

#include "detail/ansi_support.hpp"
#include "detail/input_mode.hpp"
#include "detail/key_reader.hpp"
#include "detail/osc.hpp"
#include "detail/terminal_size.hpp"

#include <memory>
#include <variant>

/**
 * @file
 * Implements the terminal class.
 */

namespace kaycxx::term {

terminal::terminal(std::ostream& stream, ansi_mode mode)
    : stream_(stream)
    , ansi_enabled_(false)
    , input_modes_(std::make_unique<detail::input_mode_stack>())
{
    switch (mode) {
        case ansi_mode::never:
            ansi_enabled_ = false;
            break;
        case ansi_mode::always:
            ansi_enabled_ = true;
            break;
        case ansi_mode::automatic:
            ansi_enabled_ = detail::supports_ansi(stream);
    }
}

terminal::~terminal() = default;

void terminal::clear_screen() {
    if (ansi_enabled_) {
        stream_ << "\033[2J";
    }
}

void terminal::clear_screen_to_end() {
    if (ansi_enabled_) {
        stream_ << "\033[J";
    }
}

void terminal::clear_screen_to_start() {
    if (ansi_enabled_) {
        stream_ << "\033[1J";
    }
}

void terminal::clear_scrollback() {
    if (ansi_enabled_) {
        stream_ << "\033[3J";
    }
}

void terminal::clear_line() {
    if (ansi_enabled_) {
        stream_ << "\033[2K";
    }
}

void terminal::clear_line_to_end() {
    if (ansi_enabled_) {
        stream_ << "\033[K";
    }
}

void terminal::clear_line_to_start() {
    if (ansi_enabled_) {
        stream_ << "\033[1K";
    }
}

void terminal::push_title(std::string_view title) {
    if (ansi_enabled_) {
        stream_ << "\033[22;2t";
        stream_ << "\033]2;";
        stream_ << detail::sanitize_osc_payload(title);
        stream_ << "\033\\";
    }
}

void terminal::pop_title() {
    if (ansi_enabled_) {
        stream_ << "\033[23;2t";
    }
}

void terminal::hide_cursor() {
    if (ansi_enabled_) {
        stream_ << "\033[?25l";
    }
}

void terminal::show_cursor() {
    if (ansi_enabled_) {
        stream_ << "\033[?25h";
    }
}

void terminal::save_cursor() {
    if (ansi_enabled_) {
        stream_ << "\033[s";
    }
}

void terminal::restore_cursor() {
    if (ansi_enabled_) {
        stream_ << "\033[u";
    }
}

void terminal::set_cursor_style(cursor_style style) {
    if (ansi_enabled_) {
        stream_ << "\033[" << static_cast<int>(style) << " q";
    }
}

void terminal::move_to(int column, int row) {
    if (ansi_enabled_) {
        stream_ << "\033[" << column << "G";
        stream_ << "\033[" << row << "d";
    }
}

void terminal::move_by(int columns, int rows) {
    if (ansi_enabled_) {
        if (columns > 0) {
            stream_ << "\033[" << columns << "C";
        } else if (columns < 0) {
            stream_ << "\033[" << -columns << "D";
        }
        if (rows > 0) {
            stream_ << "\033[" << rows << "B";
        } else if (rows < 0) {
            stream_ << "\033[" << -rows << "A";
        }
    }
}

void terminal::move_to_column(int column) {
    if (ansi_enabled_) {
        stream_ << "\033[" << column << "G";
    }
}

void terminal::enable_wrap() {
    if (ansi_enabled_) {
        stream_ << "\033[?7h";
    }
}

void terminal::disable_wrap() {
    if (ansi_enabled_) {
        stream_ << "\033[?7l";
    }
}

void terminal::enter_alternate_screen() {
    if (ansi_enabled_) {
        stream_ << "\033[?1049h";
    }
}

void terminal::leave_alternate_screen() {
    if (ansi_enabled_) {
        stream_ << "\033[?1049l";
    }
}

terminal_size terminal::size() const {
    return detail::read_terminal_size(stream_);
}

bool terminal::push_input_mode(input_mode mode) {
    return input_modes_->push(mode);
}

bool terminal::pop_input_mode() {
    return input_modes_->pop();
}

std::string terminal::read_key() {
    auto pushed_raw_mode = false;
    if (!input_modes_->is_raw()) {
        if (!input_modes_->push(input_mode::raw)) {
            return {};
        }
        pushed_raw_mode = true;
    }

    try {
        auto key = detail::read_key();

        if (pushed_raw_mode) {
            input_modes_->pop();
        }

        return key;
    } catch (...) {
        if (pushed_raw_mode) {
            input_modes_->pop();
        }
        throw;
    }
}

void terminal::insert_line() {
    if (ansi_enabled_) {
        stream_ << "\033[L";
    }
}

void terminal::insert_lines(int lines) {
    if (ansi_enabled_ && lines > 0) {
        stream_ << "\033[" << lines << "L";
    }
}

void terminal::delete_line() {
    if (ansi_enabled_) {
        stream_ << "\033[M";
    }
}

void terminal::delete_lines(int lines) {
    if (ansi_enabled_ && lines > 0) {
        stream_ << "\033[" << lines << "M";
    }
}

void terminal::clear_cell() {
    if (ansi_enabled_) {
        stream_ << "\033[X";
    }
}

void terminal::clear_cells(int cells) {
    if (ansi_enabled_ && cells > 0) {
        stream_ << "\033[" << cells << "X";
    }
}

void terminal::insert_cell() {
    if (ansi_enabled_) {
        stream_ << "\033[@";
    }
}

void terminal::insert_cells(int cells) {
    if (ansi_enabled_ && cells > 0) {
        stream_ << "\033[" << cells << "@";
    }
}

void terminal::delete_cell() {
    if (ansi_enabled_) {
        stream_ << "\033[P";
    }
}

void terminal::delete_cells(int cells) {
    if (ansi_enabled_ && cells > 0) {
        stream_ << "\033[" << cells << "P";
    }
}

void terminal::set_scroll_region(int top, int bottom) {
    if (ansi_enabled_ && top > 0 && bottom >= top) {
        stream_ << "\033[" << top << ';' << bottom << 'r';
    }
}

void terminal::reset_scroll_region() {
    if (ansi_enabled_) {
        stream_ << "\033[r";
    }
}

void terminal::scroll_up() {
    if (ansi_enabled_) {
        stream_ << "\033[S";
    }
}

void terminal::scroll_up(int lines) {
    if (ansi_enabled_ && lines > 0) {
        stream_ << "\033[" << lines << 'S';
    }
}

void terminal::scroll_down() {
    if (ansi_enabled_) {
        stream_ << "\033[T";
    }
}

void terminal::scroll_down(int lines) {
    if (ansi_enabled_ && lines > 0) {
        stream_ << "\033[" << lines << 'T';
    }
}

terminal& terminal::operator<<(sgr value) {
    if (ansi_enabled_) {
        stream_ << "\033[" << value.code << 'm';
    }
    return *this;
}

terminal& terminal::operator<<(color_style style) {
    if (ansi_enabled_) {
        std::visit([&](auto const& value) {
            write_color_value(value, style);
        }, style.value);
    }
    return *this;
}

terminal& terminal::operator<<(hyperlink const& value) {
    auto const url = detail::sanitize_osc_payload(value.url);
    auto const text = detail::sanitize_osc_payload(value.text);

    if (ansi_enabled_) {
        stream_
            << "\033]8;;" << url << "\033\\"
            << text
            << "\033]8;;\033\\";
    } else {
        stream_ << text << " (" << url << ')';
    }

    return *this;
}

terminal& terminal::operator<<(std::ostream& (*manipulator)(std::ostream&)) {
    manipulator(stream_);
    return *this;
}

terminal& terminal::operator<<(std::ios& (*manipulator)(std::ios&)) {
    manipulator(stream_);
    return *this;
}

terminal& terminal::operator<<(std::ios_base& (*manipulator)(std::ios_base&)) {
    manipulator(stream_);
    return *this;
}

void terminal::write_color_value(basic_color value, color_style style) {
    auto base = value.bright ? style.bright_base : style.basic_base;
    stream_ << "\033[" << base + static_cast<int>(value.index) << 'm';
}

void terminal::write_color_value(rgb_color value, color_style style) {
    stream_
        << "\033[" << style.extended_base << ";2;"
        << static_cast<int>(value.red) << ';'
        << static_cast<int>(value.green) << ';'
        << static_cast<int>(value.blue) << 'm';
}

void terminal::write_color_value(indexed_color value, color_style style) {
    stream_
        << "\033[" << style.extended_base
        << ";5;"
        << static_cast<int>(value.index)
        << 'm';
}

} // namespace kaycxx::term
