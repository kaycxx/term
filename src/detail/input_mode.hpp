// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file
 * Defines the internal input mode stack.
 */

#include "input_state.hpp"

#include <vector>

#include <kaycxx/term/input_mode.hpp>

namespace kaycxx::term::detail {

/**
 * Stack of saved terminal input states.
 *
 * The stack stores native operating-system input state snapshots and restores them in last-in-first-out order.
 */
class input_mode_stack {
public:
    /**
     * Saves the current input state and applies a new input mode.
     *
     * @param mode  Input mode to apply.
     * @returns True when the state was saved and the requested mode was applied.
     */
    bool push(input_mode mode);

    /**
     * Restores the most recently pushed input state.
     *
     * @returns True when a state was restored.
     */
    bool pop();

    /**
     * Checks whether the current input state behaves like raw mode.
     *
     * @returns True when raw input is currently active.
     */
    bool is_raw() const;

private:
    /** Saved native input states. */
    std::vector<input_state> stack_;

    /**
     * Reads the current native input state.
     *
     * @param state  Destination for the current input state.
     * @returns True when the state was read.
     */
    static bool read(input_state& state);

    /**
     * Writes a native input state.
     *
     * @param state  Input state to apply.
     * @returns True when the state was written.
     */
    static bool write(input_state const& state);

    /**
     * Applies a logical input mode to a native input state snapshot.
     *
     * @param state  Native input state to modify.
     * @param mode   Input mode to apply.
     */
    static void apply(input_state& state, input_mode mode);

    /**
     * Compares two native input states.
     *
     * @param left   First input state.
     * @param right  Second input state.
     * @returns True when both states are equivalent.
     */
    static bool equals(input_state const& left, input_state const& right);

    /**
     * Checks whether a native input state behaves like raw mode.
     *
     * @param state  Native input state to inspect.
     * @returns True when the state represents raw input.
     */
    static bool is_raw(input_state const& state);
};

} // namespace kaycxx::term::detail
