#include "includes/utils.h"

namespace utils {
void left_trim(std::string& s) {
    const auto pos = s.find_first_not_of(" \t\n\r\f\v");
    if (pos == std::string::npos)
        s.clear();
    else
        s.erase(0, pos);
}

void right_trim(std::string& s) {
    const auto pos = s.find_last_not_of(" \t\n\r\f\v");
    if (pos == std::string::npos)
        s.clear();
    else
        s.erase(pos + 1);
}

void trim(std::string& s) {
    left_trim(s);
    right_trim(s);
}
}  // namespace utils