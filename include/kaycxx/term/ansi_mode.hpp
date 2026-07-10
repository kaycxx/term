// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines ANSI control sequence handling modes.
 */

namespace kaycxx::term {

/**
 * Selects which ANSI/VT formatting and terminal commands are emitted.
 */
enum class ansi_mode {
    /** Detect terminal command support and honor color environment variables for formatting. */
    automatic,

    /** Do not emit ANSI/VT formatting or terminal commands. */
    none,

    /** Emit ANSI/VT formatting but no terminal commands. */
    formatting_only,

    /** Emit ANSI/VT terminal commands but no formatting. */
    commands_only,

    /** Emit all ANSI/VT formatting and terminal commands. */
    full
};

} // namespace kaycxx::term
