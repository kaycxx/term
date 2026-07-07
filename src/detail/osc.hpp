// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Declares internal OSC helpers.
 */

#include <string>
#include <string_view>

namespace kaycxx::term::detail {

/**
 * Sanitizes a string for use as an OSC payload.
 *
 * ESC and BEL are removed because they can terminate or inject terminal control sequences inside OSC strings.
 *
 * @param value  String to sanitize.
 * @returns Sanitized OSC payload.
 */
std::string sanitize_osc_payload(std::string_view value);

} // namespace kaycxx::term::detail
