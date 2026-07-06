// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include "key_reader.hpp"

#include <cerrno>
#include <poll.h>
#include <unistd.h>

/**
 * @file
 * Implements POSIX key byte reading.
 */

namespace {

/**
 * Checks whether stdin has data ready to read.
 *
 * @param timeout_ms  Timeout in milliseconds.
 * @returns True when stdin can be read without blocking.
 */
bool has_stdin_data(int timeout_ms) {
    auto descriptor = pollfd{
        .fd = STDIN_FILENO,
        .events = POLLIN,
        .revents = 0
    };

    while (true) {
        auto result = poll(&descriptor, 1, timeout_ms);

        if (result > 0) {
            return (descriptor.revents & POLLIN) != 0;
        }
        if (result == 0) {
            return false;
        }
        if (errno != EINTR) {
            return false;
        }
    }
}

} // namespace

namespace kaycxx::term::detail {

bool read_key_byte(char& value) {
    while (true) {
        auto result = ::read(STDIN_FILENO, &value, 1);

        if (result == 1) {
            return true;
        }
        if (result == -1 && errno == EINTR) {
            continue;
        }
        return false;
    }
}

bool read_key_byte(char& value, int timeout_ms) {
    return has_stdin_data(timeout_ms) && read_key_byte(value);
}

} // namespace kaycxx::term::detail
