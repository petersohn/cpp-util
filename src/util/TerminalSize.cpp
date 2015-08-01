#include "util/TerminalSize.hpp"

#include <sys/ioctl.h>


namespace util {

TerminalSize getTerminalSize(int fd)
{
    ::winsize size;
    int result = ::ioctl(fd, TIOCGWINSZ, &size);

    TerminalSize terminalSize;

    if (result == 0) {
        terminalSize.width = size.ws_col;
        terminalSize.height = size.ws_row;
    }
    return terminalSize;
}

} // namespace util

