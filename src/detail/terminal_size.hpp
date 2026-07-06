// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Declares internal terminal size detection.
 */

#include <iosfwd>

#include <kaycxx/term/terminal_size.hpp>

namespace kaycxx::term::detail {

/**
 * Reads the terminal size for a stream.
 *
 * @param stream  Stream whose terminal size should be read.
 * @returns Terminal size, or zero columns and rows when unavailable.
 */
terminal_size read_terminal_size(std::ostream& stream);

} // namespace kaycxx::term::detail
