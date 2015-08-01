#include "util/TerminalSize.hpp"

#include <sys/ioctl.h>
#include <unistd.h>


namespace util {

TerminalSize getTerminalSize()
{
    ::winsize size;
    ::ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    TerminalSize result;
    result.width = size.ws_col;
    result.height = size.ws_row;
    return result;
}

} // namespace util

