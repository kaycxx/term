// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include <cstdlib>
#include <sstream>
#include <stdexcept>

#include <kaycxx/assert.hpp>
#include <kaycxx/term.hpp>
#include <kaycxx/test.hpp>

#include "../src/detail/ansi_support.hpp"

using namespace kaycxx::assert;
using namespace kaycxx::term;
using namespace kaycxx::test;

namespace {

void set_environment_variable(char const* name, bool enabled) {
#if defined(_WIN32)
    auto const result = _putenv_s(name, enabled ? "1" : "");
#else
    auto const result = enabled ? setenv(name, "1", 1) : unsetenv(name);
#endif
    if (result != 0) {
        throw std::runtime_error("Unable to configure test environment");
    }
}

void set_color_environment(bool force_color, bool no_color) {
    set_environment_variable("FORCE_COLOR", force_color);
    set_environment_variable("NO_COLOR", no_color);
}

} // namespace

suite("ansi_mode") {
    before_each([] {
        set_color_environment(false, false);
    });

    it("disables formatting and commands for unknown streams in automatic mode", [] {
        auto output = std::ostringstream();
        auto term = terminal(output, ansi_mode::automatic);

        term << bold << "text";
        term.clear_line();

        assert_false(term.formatting_enabled());
        assert_false(term.commands_enabled());
        assert_equal(output.str(), "text");
    });

    it("enables only formatting with FORCE_COLOR in automatic mode", [] {
        set_color_environment(true, false);
        auto output = std::ostringstream();
        auto term = terminal(output, ansi_mode::automatic);

        term << bold << "text";
        term.clear_line();

        assert_true(term.formatting_enabled());
        assert_false(term.commands_enabled());
        assert_equal(output.str(), "\033[1mtext");
    });

    it("disables formatting with NO_COLOR", [] {
        set_color_environment(false, true);
        auto output = std::ostringstream();
        auto term = terminal(output, ansi_mode::automatic);

        assert_false(term.formatting_enabled());
        assert_false(term.commands_enabled());
        assert_false(kaycxx::term::detail::resolve_formatting_enabled(true));
    });

    it("gives FORCE_COLOR priority over NO_COLOR", [] {
        set_color_environment(true, true);
        auto output = std::ostringstream();
        auto term = terminal(output, ansi_mode::automatic);

        assert_true(term.formatting_enabled());
        assert_false(term.commands_enabled());
    });

    it("uses terminal support when no color environment variable is set", [] {
        assert_false(kaycxx::term::detail::resolve_formatting_enabled(false));
        assert_true(kaycxx::term::detail::resolve_formatting_enabled(true));
    });

    it("emits plain text only in none mode", [] {
        set_color_environment(true, false);
        auto output = std::ostringstream();
        auto term = terminal(output, ansi_mode::none);

        term << bold;
        term.clear_line();
        term << "text";

        assert_false(term.formatting_enabled());
        assert_false(term.commands_enabled());
        assert_equal(output.str(), "text");
    });

    it("emits only formatting in formatting_only mode", [] {
        set_color_environment(false, true);
        auto output = std::ostringstream();
        auto term = terminal(output, ansi_mode::formatting_only);

        term << bold;
        term.clear_line();
        term << "text";

        assert_true(term.formatting_enabled());
        assert_false(term.commands_enabled());
        assert_equal(output.str(), "\033[1mtext");
    });

    it("emits only commands in commands_only mode", [] {
        set_color_environment(true, false);
        auto output = std::ostringstream();
        auto term = terminal(output, ansi_mode::commands_only);

        term << bold;
        term.clear_line();
        term << "text";

        assert_false(term.formatting_enabled());
        assert_true(term.commands_enabled());
        assert_equal(output.str(), "\033[2Ktext");
    });

    it("emits formatting and commands in full mode", [] {
        set_color_environment(false, true);
        auto output = std::ostringstream();
        auto term = terminal(output, ansi_mode::full);

        term << bold;
        term.clear_line();
        term << "text";

        assert_true(term.formatting_enabled());
        assert_true(term.commands_enabled());
        assert_equal(output.str(), "\033[1m\033[2Ktext");
    });
}
