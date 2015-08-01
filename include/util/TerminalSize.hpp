#ifndef UTIL_TERMINALSIZE_HPP
#define UTIL_TERMINALSIZE_HPP

namespace util {

struct TerminalSize {
    int width;
    int height;
};

TerminalSize getTerminalSize();

} // namespace util


#endif // UTIL_TERMINALSIZE_HPP
