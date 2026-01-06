#include "includes/json.h"

namespace json {
void write(const json_t& data, const std::string& path) {
    std::ofstream file(path);
    file << std::setw(4) << data << '\n';
    file.close();
}

json_t read(const std::string& path) {
    std::ifstream file(path);
    json_t data = json_t::parse(file);
    file.close();

    return data;
}
}  // namespace json
