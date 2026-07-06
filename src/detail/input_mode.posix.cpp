// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include "input_mode.hpp"

#include <termios.h>
#include <unistd.h>

/**
 * @file
 * Implements POSIX terminal input mode handling.
 */

namespace kaycxx::term::detail {

bool input_mode_stack::read(input_state& state) {
    return tcgetattr(STDIN_FILENO, &state.mode) == 0;
}

bool input_mode_stack::write(input_state const& state) {
    return tcsetattr(STDIN_FILENO, TCSANOW, &state.mode) == 0;
}

void input_mode_stack::apply(input_state& state, input_mode mode) {
    switch (mode) {
        case input_mode::normal:
            state.mode.c_iflag |= ICRNL | IXON;
            state.mode.c_oflag |= OPOST;
            state.mode.c_lflag |= ECHO | ICANON | IEXTEN | ISIG;
            state.mode.c_cflag |= CS8;
            state.mode.c_cc[VMIN] = 1;
            state.mode.c_cc[VTIME] = 0;
            break;

        case input_mode::raw:
            state.mode.c_iflag &= static_cast<tcflag_t>(~(BRKINT | ICRNL | INPCK | ISTRIP | IXON));
            state.mode.c_oflag &= static_cast<tcflag_t>(~OPOST);
            state.mode.c_lflag &= static_cast<tcflag_t>(~(ECHO | ICANON | IEXTEN | ISIG));
            state.mode.c_cflag |= CS8;
            state.mode.c_cc[VMIN] = 1;
            state.mode.c_cc[VTIME] = 0;
            break;
    }
}

bool input_mode_stack::equals(input_state const& left, input_state const& right) {
    return left.mode.c_iflag == right.mode.c_iflag
        && left.mode.c_oflag == right.mode.c_oflag
        && left.mode.c_cflag == right.mode.c_cflag
        && left.mode.c_lflag == right.mode.c_lflag
        && left.mode.c_line == right.mode.c_line
        && left.mode.c_cc[VMIN] == right.mode.c_cc[VMIN]
        && left.mode.c_cc[VTIME] == right.mode.c_cc[VTIME];
}

bool input_mode_stack::is_raw(input_state const& state) {
    return (state.mode.c_lflag & static_cast<tcflag_t>(ICANON | ECHO)) == 0;
}

} // namespace kaycxx::term::detail
