// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include "input_mode.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/**
 * @file
 * Implements Windows console input mode handling.
 */

namespace kaycxx::term::detail {

bool input_mode_stack::read(input_state& state) {
    auto handle = GetStdHandle(STD_INPUT_HANDLE);
    if (handle == nullptr || handle == INVALID_HANDLE_VALUE) {
        return false;
    }

    return GetConsoleMode(handle, &state.mode) != 0;
}

bool input_mode_stack::write(input_state const& state) {
    auto handle = GetStdHandle(STD_INPUT_HANDLE);
    if (handle == nullptr || handle == INVALID_HANDLE_VALUE) {
        return false;
    }

    return SetConsoleMode(handle, state.mode) != 0;
}

void input_mode_stack::apply(input_state& state, input_mode mode) {
    switch (mode) {
        case input_mode::normal:
            state.mode |= ENABLE_PROCESSED_INPUT;
            state.mode |= ENABLE_LINE_INPUT;
            state.mode |= ENABLE_ECHO_INPUT;
            state.mode &= static_cast<unsigned long>(~ENABLE_VIRTUAL_TERMINAL_INPUT);
            break;

        case input_mode::raw:
            state.mode &= static_cast<unsigned long>(~ENABLE_PROCESSED_INPUT);
            state.mode &= static_cast<unsigned long>(~ENABLE_LINE_INPUT);
            state.mode &= static_cast<unsigned long>(~ENABLE_ECHO_INPUT);
            state.mode &= static_cast<unsigned long>(~ENABLE_VIRTUAL_TERMINAL_INPUT);
            break;
    }
}

bool input_mode_stack::equals(input_state const& left, input_state const& right) {
    return left.mode == right.mode;
}

bool input_mode_stack::is_raw(input_state const& state) {
    return (state.mode & ENABLE_LINE_INPUT) == 0
        && (state.mode & ENABLE_ECHO_INPUT) == 0;
}

} // namespace kaycxx::term::detail
