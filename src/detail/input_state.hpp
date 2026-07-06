// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines the internal native input state wrapper.
 */

#ifndef _WIN32
#include <termios.h>
#endif

namespace kaycxx::term::detail {

/**
 * Native terminal input state.
 */
struct input_state {
#ifdef _WIN32

    /** Windows console mode flags. */
    unsigned long mode;
#else

    /** POSIX terminal mode state. */
    ::termios mode;
#endif
};

} // namespace kaycxx::term::detail
