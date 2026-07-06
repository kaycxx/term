// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines ANSI control sequence handling modes.
 */

namespace kaycxx::term {

/**
 * Controls when ANSI/VT terminal control sequences are emitted.
 */
enum class ansi_mode {
    /** Detect terminal support automatically and honor color environment variables. */
    automatic,

    /** Always emit ANSI/VT control sequences. */
    always,

    /** Never emit ANSI/VT control sequences. */
    never
};

} // namespace kaycxx::term
