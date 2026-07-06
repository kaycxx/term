// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines foreground and background color styling helpers.
 */

#include <kaycxx/term/color.hpp>

namespace kaycxx::term {

/**
 * Color style command written to a terminal.
 */
struct color_style {
    /** Color value to render. */
    color value;

    /** Base SGR code for non-bright basic colors. */
    int basic_base;

    /** Base SGR code for bright basic colors. */
    int bright_base;

    /** Extended SGR selector for indexed and RGB colors. */
    int extended_base;
};

/**
 * Creates a foreground color style.
 *
 * @param value  Color value to use as foreground.
 * @returns Foreground color style.
 */
inline constexpr color_style fg(color value) noexcept {
    return color_style{
        .value = value,
        .basic_base = 30,
        .bright_base = 90,
        .extended_base = 38
    };
}

/**
 * Creates a background color style.
 *
 * @param value  Color value to use as background.
 * @returns Background color style.
 */
inline constexpr color_style bg(color value) noexcept {
    return color_style{
        .value = value,
        .basic_base = 40,
        .bright_base = 100,
        .extended_base = 48
    };
}

} // namespace kaycxx::term
