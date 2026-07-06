// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Declares internal key reading helpers.
 */

#include <string>

namespace kaycxx::term::detail {

/**
 * Reads one raw key byte, blocking until input is available.
 *
 * @param value  Destination for the read byte.
 * @returns True when one byte was read.
 */
bool read_key_byte(char& value);

/**
 * Reads one raw key byte with a timeout.
 *
 * @param value       Destination for the read byte.
 * @param timeout_ms  Timeout in milliseconds.
 * @returns True when one byte was read before the timeout.
 */
bool read_key_byte(char& value, int timeout_ms);

/**
 * Reads one key as a raw byte sequence.
 *
 * @returns Raw key byte sequence, or an empty string on failure.
 */
std::string read_key();

} // namespace kaycxx::term::detail
