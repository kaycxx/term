// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines terminal input modes.
 */

namespace kaycxx::term {

/**
 * Terminal input mode.
 */
enum class input_mode {
    /** Canonical line input with echo and normal processing. */
    normal,

    /** Raw key input without line buffering or echo. */
    raw
};

} // namespace kaycxx::term
