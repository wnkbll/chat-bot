#include "includes/runtime.h"

bool is_valid_path(const std::filesystem::path& path) {
    if (std::filesystem::is_directory(path)) {
        std::printf("%s is directory, use ./data.json instead\n", path.c_str());
        return false;
    }

    if (path.extension() != ".json") {
        std::printf("%s is not json file, use ./data.json instead\n", path.c_str());
        return false;
    }

    return true;
}

int main(int argc, char** argv) {
    std::filesystem::path data_path = "data.json";

    if (argc > 1 && is_valid_path(argv[1])) {
        data_path = argv[1];
    }

    Runtime runtime(data_path);
    runtime.run();
}
