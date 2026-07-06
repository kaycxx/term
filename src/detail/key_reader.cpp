// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include "key_reader.hpp"

#include <cstddef>
#include <string>

/**
 * @file
 * Implements platform-independent key sequence assembly.
 */

namespace {

/** Timeout used to distinguish Escape from Escape-prefixed key sequences. */
constexpr int escape_timeout_ms = 25;

/**
 * Returns the expected UTF-8 byte sequence length from the first byte.
 *
 * @param byte  First byte of a potential UTF-8 sequence.
 * @returns Expected byte count.
 */
std::size_t utf8_sequence_size(unsigned char byte) {
    if ((byte & 0b1000'0000) == 0) {
        return 1;
    }
    if ((byte & 0b1110'0000) == 0b1100'0000) {
        return 2;
    }
    if ((byte & 0b1111'0000) == 0b1110'0000) {
        return 3;
    }
    if ((byte & 0b1111'1000) == 0b1111'0000) {
        return 4;
    }
    return 1;
}

/**
 * Checks whether a byte is a CSI final byte.
 *
 * @param value  Byte to inspect.
 * @returns True when the byte terminates a CSI sequence.
 */
bool is_csi_final(char value) {
    auto byte = static_cast<unsigned char>(value);
    return byte >= 0x40 && byte <= 0x7e;
}

/**
 * Reads the remaining bytes of a UTF-8 sequence.
 *
 * @param result      String receiving additional bytes.
 * @param first_byte  First byte already present in the result.
 */
void read_utf8_tail(std::string& result, unsigned char first_byte) {
    auto size = utf8_sequence_size(first_byte);

    for (std::size_t i = 1; i < size; ++i) {
        char value{};
        if (!kaycxx::term::detail::read_key_byte(value)) {
            return;
        }
        result += value;
    }
}

} // namespace

namespace kaycxx::term::detail {

std::string read_key() {
    auto result = std::string();

    char first{};
    if (!read_key_byte(first)) {
        return result;
    }

    result += first;

    if (first != '\033') {
        read_utf8_tail(result, static_cast<unsigned char>(first));
        return result;
    }

    char second{};
    if (!read_key_byte(second, escape_timeout_ms)) {
        return result;
    }

    result += second;

    if (second == '[') {
        char value{};
        while (read_key_byte(value, escape_timeout_ms)) {
            result += value;

            if (is_csi_final(value)) {
                break;
            }
        }
        return result;
    }

    if (second == 'O') {
        char value{};
        if (read_key_byte(value, escape_timeout_ms)) {
            result += value;
        }
        return result;
    }

    read_utf8_tail(result, static_cast<unsigned char>(second));
    return result;
}

} // namespace kaycxx::term::detail
