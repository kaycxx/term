// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include <iomanip>
#include <sstream>

#include <kaycxx/assert.hpp>
#include <kaycxx/term.hpp>
#include <kaycxx/test.hpp>

using namespace kaycxx::assert;
using namespace kaycxx::term;
using namespace kaycxx::test;

suite("terminal") {
    describe("formatting", [] {
        it("writes SGR styles", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::formatting_only);

            term << reset << bold << dim << italic << underline << inverse << normal_intensity << no_italic << no_underline << no_inverse << reset_foreground
                 << reset_background;

            assert_equal(
                output.str(),
                "\033[0m"
                "\033[1m"
                "\033[2m"
                "\033[3m"
                "\033[4m"
                "\033[7m"
                "\033[22m"
                "\033[23m"
                "\033[24m"
                "\033[27m"
                "\033[39m"
                "\033[49m"
            );
        });

        it("writes basic foreground colors", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::formatting_only);

            term << fg(black) << fg(red) << fg(green) << fg(yellow) << fg(blue) << fg(magenta) << fg(cyan) << fg(white) << fg(bright_black) << fg(bright_red)
                 << fg(bright_green) << fg(bright_yellow) << fg(bright_blue) << fg(bright_magenta) << fg(bright_cyan) << fg(bright_white);

            assert_equal(
                output.str(),
                "\033[30m"
                "\033[31m"
                "\033[32m"
                "\033[33m"
                "\033[34m"
                "\033[35m"
                "\033[36m"
                "\033[37m"
                "\033[90m"
                "\033[91m"
                "\033[92m"
                "\033[93m"
                "\033[94m"
                "\033[95m"
                "\033[96m"
                "\033[97m"
            );
        });

        it("writes basic background colors", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::formatting_only);

            term << bg(black) << bg(red) << bg(green) << bg(yellow) << bg(blue) << bg(magenta) << bg(cyan) << bg(white) << bg(bright_black) << bg(bright_red)
                 << bg(bright_green) << bg(bright_yellow) << bg(bright_blue) << bg(bright_magenta) << bg(bright_cyan) << bg(bright_white);

            assert_equal(
                output.str(),
                "\033[40m"
                "\033[41m"
                "\033[42m"
                "\033[43m"
                "\033[44m"
                "\033[45m"
                "\033[46m"
                "\033[47m"
                "\033[100m"
                "\033[101m"
                "\033[102m"
                "\033[103m"
                "\033[104m"
                "\033[105m"
                "\033[106m"
                "\033[107m"
            );
        });

        it("writes indexed and RGB colors", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::formatting_only);

            term << fg(indexed(0)) << bg(indexed(255)) << fg(rgb(1, 2, 3)) << bg(rgb(254, 128, 0));

            assert_equal(output.str(), "\033[38;5;0m\033[48;5;255m\033[38;2;1;2;3m\033[48;2;254;128;0m");
        });
    });

    describe("screen commands", [] {
        it("writes screen and line clearing commands", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::commands_only);

            term.clear_screen();
            term.clear_screen_to_end();
            term.clear_screen_to_start();
            term.clear_scrollback();
            term.clear_line();
            term.clear_line_to_end();
            term.clear_line_to_start();

            assert_equal(output.str(), "\033[2J\033[J\033[1J\033[3J\033[2K\033[K\033[1K");
        });

        it("writes line insertion and deletion commands", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::commands_only);

            term.insert_line();
            term.insert_lines(3);
            term.delete_line();
            term.delete_lines(4);

            assert_equal(output.str(), "\033[L\033[3L\033[M\033[4M");
        });

        it("writes cell clearing, insertion, and deletion commands", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::commands_only);

            term.clear_cell();
            term.clear_cells(2);
            term.insert_cell();
            term.insert_cells(3);
            term.delete_cell();
            term.delete_cells(4);

            assert_equal(output.str(), "\033[X\033[2X\033[@\033[3@\033[P\033[4P");
        });

        it("writes scrolling commands", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::commands_only);

            term.set_scroll_region(2, 10);
            term.reset_scroll_region();
            term.scroll_up();
            term.scroll_up(3);
            term.scroll_down();
            term.scroll_down(4);

            assert_equal(output.str(), "\033[2;10r\033[r\033[S\033[3S\033[T\033[4T");
        });

        it("ignores invalid counts and scrolling regions", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::commands_only);

            term.insert_lines(0);
            term.insert_lines(-1);
            term.delete_lines(0);
            term.delete_lines(-1);
            term.clear_cells(0);
            term.clear_cells(-1);
            term.insert_cells(0);
            term.insert_cells(-1);
            term.delete_cells(0);
            term.delete_cells(-1);
            term.set_scroll_region(0, 10);
            term.set_scroll_region(5, 4);
            term.scroll_up(0);
            term.scroll_up(-1);
            term.scroll_down(0);
            term.scroll_down(-1);

            assert_equal(output.str(), "");
        });

        it("writes wrap and alternate screen commands", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::commands_only);

            term.enable_wrap();
            term.disable_wrap();
            term.enter_alternate_screen();
            term.leave_alternate_screen();

            assert_equal(output.str(), "\033[?7h\033[?7l\033[?1049h\033[?1049l");
        });
    });

    describe("cursor commands", [] {
        it("writes cursor visibility and position commands", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::commands_only);

            term.hide_cursor();
            term.show_cursor();
            term.save_cursor();
            term.restore_cursor();
            term.move_to(10, 20);
            term.move_by(3, -4);
            term.move_by(-5, 6);
            term.move_by(0, 0);
            term.move_to_column(7);

            assert_equal(output.str(), "\033[?25l\033[?25h\033[s\033[u\033[10G\033[20d\033[3C\033[4A\033[5D\033[6B\033[7G");
        });

        it("writes all cursor styles", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::commands_only);

            term.set_cursor_style(cursor_style::standard);
            term.set_cursor_style(cursor_style::blinking_block);
            term.set_cursor_style(cursor_style::steady_block);
            term.set_cursor_style(cursor_style::blinking_underline);
            term.set_cursor_style(cursor_style::steady_underline);
            term.set_cursor_style(cursor_style::blinking_bar);
            term.set_cursor_style(cursor_style::steady_bar);

            assert_equal(output.str(), "\033[0 q\033[1 q\033[2 q\033[3 q\033[4 q\033[5 q\033[6 q");
        });
    });

    describe("OSC commands", [] {
        it("writes title commands", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::commands_only);

            term.push_title("Example");
            term.pop_title();

            assert_equal(output.str(), "\033[22;2t\033]2;Example\033\\\033[23;2t");
        });

        it("sanitizes title payloads", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::commands_only);

            term.push_title("A\033B\aC");

            assert_equal(output.str(), "\033[22;2t\033]2;ABC\033\\");
        });

        it("writes terminal hyperlinks", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::commands_only);

            term << kaycxx::term::link("https://example.com", "Example");

            assert_equal(output.str(), "\033]8;;https://example.com\033\\Example\033]8;;\033\\");
        });

        it("writes plain hyperlinks when commands are disabled", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::none);

            term << kaycxx::term::link("https://example.com", "Example");

            assert_equal(output.str(), "Example (https://example.com)");
        });

        it("sanitizes hyperlink payloads", [] {
            auto output = std::ostringstream();
            auto term = terminal(output, ansi_mode::commands_only);

            term << kaycxx::term::link("https://example.com/\033\a", "Ex\033\aample");

            assert_equal(output.str(), "\033]8;;https://example.com/\033\\Example\033]8;;\033\\");
        });
    });

    it("writes ordinary values and stream manipulators without ANSI output", [] {
        auto output = std::ostringstream();
        auto term = terminal(output, ansi_mode::none);

        term << std::boolalpha << true << ' ' << std::hex << 255 << '\n';

        assert_equal(output.str(), "true ff\n");
    });
}
