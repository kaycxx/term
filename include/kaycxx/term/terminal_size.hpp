// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines terminal size values.
 */

namespace kaycxx::term {

/**
 * Terminal dimensions in character cells.
 */
struct terminal_size {
    /** Terminal width in columns. */
    int columns;

    /** Terminal height in rows. */
    int rows;
};

} // namespace kaycxx::term
