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

void replace_line(int pos, const std::string& line) {
    std::cout << std::format("\x1b[{}A", pos);
    std::cout << "\x1b[2K";
    std::cout << line;
    std::cout << std::format("\x1b[{}B", pos);
    std::cout << "\x1b[0G";
}

bool parse_args(const std::string& args, std::string& question, std::string& answer) {
    int pos_opener = args.find("\"");
    int pos_closer = args.find("\"", pos_opener + 1);
    if (pos_opener == std::string::npos || pos_closer == std::string::npos) {
        return false;
    }
    question = args.substr(pos_opener + 1, pos_closer - pos_opener - 1);

    pos_opener = args.find("\"", pos_closer + 1);
    pos_closer = args.find("\"", pos_opener + 1);
    if (pos_opener == std::string::npos || pos_closer == std::string::npos) {
        return false;
    }
    answer = args.substr(pos_opener + 1, pos_closer - pos_opener - 1);

    return true;
}
}  // namespace utils
