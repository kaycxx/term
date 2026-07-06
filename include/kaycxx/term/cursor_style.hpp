// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines terminal cursor style values.
 */

namespace kaycxx::term {

/**
 * Terminal cursor display style.
 */
enum class cursor_style {
    /** Terminal-defined standard cursor style. */
    standard = 0,

    /** Blinking block cursor. */
    blinking_block = 1,

    /** Steady block cursor. */
    steady_block = 2,

    /** Blinking underline cursor. */
    blinking_underline = 3,

    /** Steady underline cursor. */
    steady_underline = 4,

    /** Blinking vertical bar cursor. */
    blinking_bar = 5,

    /** Steady vertical bar cursor. */
    steady_bar = 6
};

} // namespace kaycxx::term
