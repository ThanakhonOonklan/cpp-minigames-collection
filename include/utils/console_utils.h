#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#include <string>

namespace Utils {

// Screen manipulation
void clearScreen();
void hideCursor();
void showCursor();
void setCursorPosition(int x, int y);

// ANSI Color definitions
namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string RED     = "\033[1;31m";
    const std::string GREEN   = "\033[1;32m";
    const std::string YELLOW  = "\033[1;33m";
    const std::string BLUE    = "\033[1;34m";
    const std::string MAGENTA = "\033[1;35m";
    const std::string CYAN    = "\033[1;36m";
    const std::string WHITE   = "\033[1;37m";
    const std::string GRAY    = "\033[1;90m";
    const std::string BRIGHT_GREEN = "\033[1;92m";
}

} // namespace Utils

#endif // CONSOLE_UTILS_H
