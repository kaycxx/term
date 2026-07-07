// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines the terminal color value type.
 */

#include <variant>

#include <kaycxx/term/basic_color.hpp>
#include <kaycxx/term/indexed_color.hpp>
#include <kaycxx/term/rgb_color.hpp>

namespace kaycxx::term {

/**
 * Terminal color value.
 *
 * A color can be one of the basic ANSI palette colors, one of the indexed 256-color palette values, or a true-color RGB value.
 */
using color = std::variant<
    basic_color,
    indexed_color,
    rgb_color
>;

} // namespace kaycxx::term
