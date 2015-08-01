#ifndef UTIL_TERMINALSIZE_HPP
#define UTIL_TERMINALSIZE_HPP

namespace util {

struct TerminalSize {
    int width = 0;
    int height = 0;
};

TerminalSize getTerminalSize(int fd);

} // namespace util


#endif // UTIL_TERMINALSIZE_HPP
