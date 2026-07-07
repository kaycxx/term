// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines the basic 16-color terminal palette.
 */

#include <cstdint>

namespace kaycxx::term {

/**
 * Basic terminal palette color.
 *
 * The index is the ANSI color slot from 0 to 7. Bright colors use the same slot together with the bright flag.
 * They are rendered as SGR colors 90 to 97 or 100 to 107.
 */
struct basic_color {
    /** ANSI color slot from 0 to 7. */
    std::uint8_t index;

    /** Whether the bright variant of the color slot should be used. */
    bool bright = false;
};

/** ANSI color slot 0 (black). */
inline constexpr basic_color black{0};

/** ANSI color slot 1 (red). */
inline constexpr basic_color red{1};

/** ANSI color slot 2 (green). */
inline constexpr basic_color green{2};

/** ANSI color slot 3 (yellow). */
inline constexpr basic_color yellow{3};

/** ANSI color slot 4 (blue). */
inline constexpr basic_color blue{4};

/** ANSI color slot 5 (magenta). */
inline constexpr basic_color magenta{5};

/** ANSI color slot 6 (cyan). */
inline constexpr basic_color cyan{6};

/** ANSI color slot 7 (white, commonly rendered as light gray). */
inline constexpr basic_color white{7};

/** Bright ANSI color slot 0 (bright black, commonly rendered as dark gray). */
inline constexpr basic_color bright_black{0, true};

/** Bright ANSI color slot 1 (bright red). */
inline constexpr basic_color bright_red{1, true};

/** Bright ANSI color slot 2 (bright green). */
inline constexpr basic_color bright_green{2, true};

/** Bright ANSI color slot 3 (bright yellow). */
inline constexpr basic_color bright_yellow{3, true};

/** Bright ANSI color slot 4 (bright blue). */
inline constexpr basic_color bright_blue{4, true};

/** Bright ANSI color slot 5 (bright magenta). */
inline constexpr basic_color bright_magenta{5, true};

/** Bright ANSI color slot 6 (bright cyan). */
inline constexpr basic_color bright_cyan{6, true};

/** Bright ANSI color slot 7 (bright white, commonly rendered as actual white). */
inline constexpr basic_color bright_white{7, true};

} // namespace kaycxx::term
