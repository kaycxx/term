// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include "ansi_support.hpp"

#include <iostream>
#include <unistd.h>

/**
 * @file
 * Implements POSIX ANSI support detection.
 */

namespace kaycxx::term::detail {

bool detect_ansi_support(std::ostream& stream) {
    if (&stream == &std::cout) {
        return isatty(STDOUT_FILENO);
    }
    if (&stream == &std::cerr || &stream == &std::clog) {
        return isatty(STDERR_FILENO);
    }
    return false;
}

} // namespace kaycxx::term::detail
