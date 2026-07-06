// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include "terminal_size.hpp"

#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

/**
 * @file
 * Implements POSIX terminal size detection.
 */

namespace {

/**
 * Returns the file descriptor for a standard stream.
 *
 * @param stream  Stream to inspect.
 * @returns File descriptor, or -1 when the stream is not a known terminal stream.
 */
int descriptor_for(std::ostream& stream) {
    if (&stream == &std::cout) {
        return STDOUT_FILENO;
    }
    if (&stream == &std::cerr || &stream == &std::clog) {
        return STDERR_FILENO;
    }
    return -1;
}

} // namespace

namespace kaycxx::term::detail {

terminal_size read_terminal_size(std::ostream& stream) {
    auto const descriptor = descriptor_for(stream);
    if (descriptor < 0) {
        return {};
    }

    auto window_size = winsize{};
    if (ioctl(descriptor, TIOCGWINSZ, &window_size) != 0) {
        return {};
    }

    return {
        .columns = static_cast<int>(window_size.ws_col),
        .rows = static_cast<int>(window_size.ws_row)
    };
}

} // namespace kaycxx::term::detail
