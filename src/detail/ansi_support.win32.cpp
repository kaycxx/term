// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include "ansi_support.hpp"

#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/**
 * @file
 * Implements Windows ANSI support detection.
 */

namespace {

/**
 * Enables virtual terminal output processing for a standard Windows handle.
 *
 * @param std_handle  Windows standard handle identifier.
 * @returns True when ANSI/VT output is supported for the handle.
 */
bool enable_ansi(DWORD std_handle) {
    auto handle = GetStdHandle(std_handle);
    if (handle == nullptr || handle == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD mode{};
    if (!GetConsoleMode(handle, &mode)) {
        return false;
    }

    if ((mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0) {
        return true;
    }

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    return SetConsoleMode(handle, mode) != 0;
}

} // namespace

namespace kaycxx::term::detail {

bool detect_ansi_support(std::ostream& stream) {
    if (&stream == &std::cout) {
        static bool const supported = enable_ansi(STD_OUTPUT_HANDLE);
        return supported;
    }
    if (&stream == &std::cerr || &stream == &std::clog) {
        static bool const supported = enable_ansi(STD_ERROR_HANDLE);
        return supported;
    }
    return false;
}

} // namespace kaycxx::term::detail
