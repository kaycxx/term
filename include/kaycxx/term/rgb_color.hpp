// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines RGB true-color terminal colors.
 */

#include <cstdint>

namespace kaycxx::term {

/**
 * RGB terminal color.
 */
struct rgb_color {
    /** Red channel. */
    std::uint8_t red;

    /** Green channel. */
    std::uint8_t green;

    /** Blue channel. */
    std::uint8_t blue;
};

/**
 * Creates an RGB terminal color.
 *
 * @param red    Red channel.
 * @param green  Green channel.
 * @param blue   Blue channel.
 * @returns RGB terminal color.
 */
inline constexpr rgb_color rgb(
    std::uint8_t red,
    std::uint8_t green,
    std::uint8_t blue
) noexcept {
    return {red, green, blue};
}

} // namespace kaycxx::term
