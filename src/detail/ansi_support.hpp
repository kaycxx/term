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
 * Checks whether ANSI/VT control sequences should be written for a stream.
 *
 * The result honors NO_COLOR and FORCE_COLOR.
 *
 * @param stream  Stream to inspect.
 * @returns True when ANSI/VT output should be enabled.
 */
bool supports_ansi(std::ostream& stream);

/**
 * Detects native ANSI/VT control sequence support for a stream.
 *
 * This is implemented per platform and does not apply environment variable
 * overrides.
 *
 * @param stream  Stream to inspect.
 * @returns True when the stream natively supports ANSI/VT output.
 */
bool detect_ansi_support(std::ostream& stream);

} // namespace kaycxx::term::detail
