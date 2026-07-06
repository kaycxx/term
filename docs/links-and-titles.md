# Links and Titles

Terminal hyperlinks and title changes use OSC sequences when ANSI output is enabled. OSC payloads are sanitized by removing ESC and BEL so user-provided text cannot terminate the OSC sequence.

```cpp
#include <kaycxx/term.hpp>

using namespace kaycxx::term;

int main() {
    auto term = terminal();

    term << link("https://github.com/kaycxx/term", "kaycxx-term") << '\n';
}
```

When ANSI output is disabled, hyperlinks are rendered as plain text followed by the URL in parentheses.

```txt
kaycxx-term (https://github.com/kaycxx/term)
```

## Titles

`push_title` asks the terminal to save the current title and then sets a new one. `pop_title` asks the terminal to restore the saved title.

```cpp
term.push_title("kaycxx-term demo");

// Run demo.

term.pop_title();
```

Title push/pop support is terminal-dependent. Unsupported terminals may ignore these sequences.
