#ifndef CHATBOT_UTILS_MODULE
#define CHATBOT_UTILS_MODULE

#include <format>
#include <iostream>
#include <string>

namespace utils {
void left_trim(std::string& s);

void right_trim(std::string& s);

void trim(std::string& s);

void replace_line(int pos, const std::string& line);

bool parse_args(const std::string& args, std::string& question);

bool parse_args(const std::string& args, std::string& question, std::string& answer);
}  // namespace utils

#endif