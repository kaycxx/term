// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines terminal hyperlink output helpers.
 */

#include <string>
#include <string_view>

namespace kaycxx::term {

/**
 * Terminal hyperlink value.
 */
struct hyperlink {
    /** Link target URL. */
    std::string url;

    /** Visible link text. */
    std::string text;
};

/**
 * Creates a terminal hyperlink value.
 *
 * @param url   Link target URL.
 * @param text  Visible link text.
 * @returns Terminal hyperlink value.
 */
inline hyperlink link(std::string_view url, std::string_view text) {
    return {
        .url = std::string(url),
        .text = std::string(text)
    };
}

} // namespace kaycxx::term
