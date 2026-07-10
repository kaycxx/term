// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include "ansi_support.hpp"

#include <cstdlib>

/**
 * @file
 * Implements common ANSI support policy.
 */

namespace {

/**
 * Checks whether an environment variable is set to a non-empty value.
 *
 * @param name  Environment variable name.
 * @returns True when the variable is present and not empty.
 */
bool has_env(char const* name) {
    auto const value = std::getenv(name);
    return value != nullptr && value[0] != '\0';
}

} // namespace

namespace kaycxx::term::detail {

bool resolve_formatting_enabled(bool fallback) {
    if (has_env("FORCE_COLOR")) {
        return true;
    }
    if (has_env("NO_COLOR")) {
        return false;
    }
    return fallback;
}

} // namespace kaycxx::term::detail
