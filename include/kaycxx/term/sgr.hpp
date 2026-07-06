// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines SGR terminal style commands.
 */

namespace kaycxx::term {

/**
 * Select Graphic Rendition command.
 */
struct sgr {
    /** SGR numeric code. */
    int code;
};

/** Reset all SGR attributes. */
inline constexpr sgr reset{0};

/** Enable bold intensity. */
inline constexpr sgr bold{1};

/** Enable dim intensity. */
inline constexpr sgr dim{2};

/** Enable italic text. */
inline constexpr sgr italic{3};

/** Enable underlined text. */
inline constexpr sgr underline{4};

/** Enable inverse colors. */
inline constexpr sgr inverse{7};

/** Reset bold and dim intensity. */
inline constexpr sgr normal_intensity{22};

/** Disable italic text. */
inline constexpr sgr no_italic{23};

/** Disable underlined text. */
inline constexpr sgr no_underline{24};

/** Disable inverse colors. */
inline constexpr sgr no_inverse{27};

/** Reset foreground color. */
inline constexpr sgr reset_foreground{39};

/** Reset background color. */
inline constexpr sgr reset_background{49};

} // namespace kaycxx::term
