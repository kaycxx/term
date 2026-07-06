// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include "terminal_size.hpp"

#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/**
 * @file
 * Implements Windows terminal size detection.
 */

namespace {

/**
 * Returns the Windows handle for a standard stream.
 *
 * @param stream  Stream to inspect.
 * @returns Windows handle, or nullptr when the stream is not a known terminal stream.
 */
HANDLE handle_for(std::ostream& stream) {
    auto handle = INVALID_HANDLE_VALUE;
    if (&stream == &std::cout) {
        handle = GetStdHandle(STD_OUTPUT_HANDLE);
    } else if (&stream == &std::cerr || &stream == &std::clog) {
        handle = GetStdHandle(STD_ERROR_HANDLE);
    }

    if (handle == nullptr || handle == INVALID_HANDLE_VALUE) {
        return nullptr;
    }
    return handle;
}

} // namespace

namespace kaycxx::term::detail {

terminal_size read_terminal_size(std::ostream& stream) {
    auto handle = handle_for(stream);
    if (handle == nullptr) {
        return {};
    }

    auto info = CONSOLE_SCREEN_BUFFER_INFO{};
    if (GetConsoleScreenBufferInfo(handle, &info) == 0) {
        return {};
    }

    return {
        .columns = info.srWindow.Right - info.srWindow.Left + 1,
        .rows = info.srWindow.Bottom - info.srWindow.Top + 1
    };
}

} // namespace kaycxx::term::detail
