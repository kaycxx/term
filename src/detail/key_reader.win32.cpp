// SPDX-FileCopyrightText: 2026 Klaus Reimer <k@ailis.de>
// SPDX-License-Identifier: MIT

#include "key_reader.hpp"

#include <cstddef>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/**
 * @file
 * Implements Windows console key reading.
 */

namespace {

/** Pending bytes from a previously read Windows key event. */
std::string pending_bytes;

/**
 * Returns the Windows standard input handle.
 *
 * @returns Standard input handle, or nullptr when unavailable.
 */
HANDLE stdin_handle() {
    auto handle = GetStdHandle(STD_INPUT_HANDLE);
    if (handle == nullptr || handle == INVALID_HANDLE_VALUE) {
        return nullptr;
    }
    return handle;
}

/**
 * Pops one byte from the pending byte queue.
 *
 * @param value  Destination for the byte.
 * @returns True when one byte was available.
 */
bool pop_pending_byte(char& value) {
    if (pending_bytes.empty()) {
        return false;
    }

    value = pending_bytes.front();
    pending_bytes.erase(0, 1);
    return true;
}

/**
 * Appends a Unicode character as UTF-8.
 *
 * @param sequence  Sequence receiving UTF-8 bytes.
 * @param value     Unicode character to append.
 * @returns True when conversion succeeded.
 */
bool append_utf8(std::string& sequence, wchar_t value) {
    auto bytes = char{};
    auto size = WideCharToMultiByte(CP_UTF8, 0, &value, 1, &bytes, 1, nullptr, nullptr);
    if (size == 1) {
        sequence += bytes;
        return true;
    }

    char buffer[4]{};
    size = WideCharToMultiByte(CP_UTF8, 0, &value, 1, buffer, static_cast<int>(sizeof(buffer)), nullptr, nullptr);
    if (size <= 0) {
        return false;
    }

    sequence.append(buffer, static_cast<std::size_t>(size));
    return true;
}

/**
 * Appends a POSIX-style key sequence for a Windows key event.
 *
 * @param sequence  Sequence receiving key bytes.
 * @param event     Windows key event to translate.
 * @returns True when the event produced a key sequence.
 */
bool append_key_sequence(std::string& sequence, KEY_EVENT_RECORD const& event) {
    if (event.bKeyDown == 0) {
        return false;
    }

    if (event.uChar.UnicodeChar != 0) {
        return append_utf8(sequence, event.uChar.UnicodeChar);
    }

    switch (event.wVirtualKeyCode) {
        case VK_ESCAPE:
            sequence = "\033";
            return true;

        case VK_UP:
            sequence = "\033[A";
            return true;
        case VK_DOWN:
            sequence = "\033[B";
            return true;
        case VK_RIGHT:
            sequence = "\033[C";
            return true;
        case VK_LEFT:
            sequence = "\033[D";
            return true;

        case VK_HOME:
            sequence = "\033[H";
            return true;
        case VK_END:
            sequence = "\033[F";
            return true;
        case VK_INSERT:
            sequence = "\033[2~";
            return true;
        case VK_DELETE:
            sequence = "\033[3~";
            return true;
        case VK_PRIOR:
            sequence = "\033[5~";
            return true;
        case VK_NEXT:
            sequence = "\033[6~";
            return true;

        case VK_F1:
            sequence = "\033OP";
            return true;
        case VK_F2:
            sequence = "\033OQ";
            return true;
        case VK_F3:
            sequence = "\033OR";
            return true;
        case VK_F4:
            sequence = "\033OS";
            return true;
        case VK_F5:
            sequence = "\033[15~";
            return true;
        case VK_F6:
            sequence = "\033[17~";
            return true;
        case VK_F7:
            sequence = "\033[18~";
            return true;
        case VK_F8:
            sequence = "\033[19~";
            return true;
        case VK_F9:
            sequence = "\033[20~";
            return true;
        case VK_F10:
            sequence = "\033[21~";
            return true;
        case VK_F11:
            sequence = "\033[23~";
            return true;
        case VK_F12:
            sequence = "\033[24~";
            return true;

        default:
            return false;
    }
}

/**
 * Reads one Windows key event and converts it to key bytes.
 *
 * @param sequence    Sequence receiving key bytes.
 * @param timeout_ms  Timeout in milliseconds, or a negative value for no timeout.
 * @returns True when a key sequence was read.
 */
bool read_key_sequence(std::string& sequence, int timeout_ms) {
    auto handle = stdin_handle();
    if (handle == nullptr) {
        return false;
    }

    auto const has_timeout = timeout_ms >= 0;
    auto const deadline = GetTickCount64() + static_cast<unsigned long long>(timeout_ms);

    while (true) {
        auto wait_time = INFINITE;
        if (has_timeout) {
            auto const now = GetTickCount64();
            if (now >= deadline) {
                return false;
            }
            wait_time = static_cast<DWORD>(deadline - now);
        }

        if (WaitForSingleObject(handle, wait_time) != WAIT_OBJECT_0) {
            return false;
        }

        auto record = INPUT_RECORD{};
        auto read = DWORD{};
        if (ReadConsoleInputW(handle, &record, 1, &read) == 0 || read != 1) {
            return false;
        }

        if (record.EventType == KEY_EVENT && append_key_sequence(sequence, record.Event.KeyEvent)) {
            return true;
        }
    }
}

} // namespace

namespace kaycxx::term::detail {

bool read_key_byte(char& value) {
    if (pop_pending_byte(value)) {
        return true;
    }

    auto sequence = std::string{};
    if (!read_key_sequence(sequence, -1) || sequence.empty()) {
        return false;
    }

    pending_bytes = sequence;
    return pop_pending_byte(value);
}

bool read_key_byte(char& value, int timeout_ms) {
    if (pop_pending_byte(value)) {
        return true;
    }

    auto sequence = std::string{};
    if (!read_key_sequence(sequence, timeout_ms) || sequence.empty()) {
        return false;
    }

    pending_bytes = sequence;
    return pop_pending_byte(value);
}

} // namespace kaycxx::term::detail
