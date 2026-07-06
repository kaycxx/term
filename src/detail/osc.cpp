// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include "osc.hpp"

/**
 * @file
 * Implements OSC helper functions.
 */

namespace kaycxx::term::detail {

std::string sanitize_osc_payload(std::string_view value) {
    auto result = std::string();
    result.reserve(value.size());

    for (auto ch : value) {
        if (ch != '\033' && ch != '\a') {
            result += ch;
        }
    }

    return result;
}

} // namespace kaycxx::term::detail
