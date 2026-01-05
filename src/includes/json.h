#ifndef CHATBOT_JSON_MODULE
#define CHATBOT_JSON_MODULE

#include <fstream>
#include <iostream>

#include "../lib/json.hpp"

using json_t = nlohmann::json;

namespace json {
void write(const json_t& data, const std::string& path);

json_t read(const std::string& path);
}  // namespace json

#endif
