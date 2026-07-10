// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Declares internal ANSI support detection.
 */

#include <ostream>

namespace kaycxx::term::detail {

/**
 * Resolves whether ANSI/VT formatting should be written.
 *
 * `FORCE_COLOR` enables formatting and takes priority over `NO_COLOR`, which disables it. The fallback is returned when neither variable is set.
 *
 * @param fallback  Value to return when no color environment variable is set.
 * @returns Resolved formatting state.
 */
bool resolve_formatting_enabled(bool fallback);

/**
 * Detects native ANSI/VT terminal command support for a stream.
 *
 * This is implemented per platform and does not apply environment variable overrides.
 *
 * @param stream  Stream to inspect.
 * @returns True when the stream natively supports ANSI/VT terminal commands.
 */
bool detect_command_support(std::ostream& stream);

} // namespace kaycxx::term::detail
