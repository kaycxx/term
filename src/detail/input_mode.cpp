// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include "input_mode.hpp"

/**
 * @file
 * Implements common input mode stack logic.
 */

namespace kaycxx::term::detail {

bool input_mode_stack::push(input_mode mode) {
    input_state previous{};
    if (!read(previous)) {
        return false;
    }

    stack_.push_back(previous);

    auto next = previous;
    apply(next, mode);

    if (equals(previous, next)) {
        return true;
    }

    if (!write(next)) {
        stack_.pop_back();
        return false;
    }

    return true;
}

bool input_mode_stack::pop() {
    if (stack_.empty()) {
        return false;
    }

    auto previous = stack_.back();

    if (!write(previous)) {
        return false;
    }

    stack_.pop_back();
    return true;
}

bool input_mode_stack::is_raw() const {
    input_state current{};
    return read(current) && is_raw(current);
}

} // namespace kaycxx::term::detail
