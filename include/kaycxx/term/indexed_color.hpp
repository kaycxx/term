// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines indexed 256-color terminal colors.
 */

#include <cstdint>

namespace kaycxx::term {

/**
 * Indexed terminal palette color.
 */
struct indexed_color {
    /** 256-color palette index. */
    std::uint8_t index;
};

/**
 * Creates an indexed terminal color.
 *
 * @param index  256-color palette index.
 * @returns Indexed terminal color.
 */
inline constexpr indexed_color indexed(std::uint8_t index) noexcept {
    return {index};
}

} // namespace kaycxx::term
