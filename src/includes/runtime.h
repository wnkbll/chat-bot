#ifndef CHATBOT_RUNTIME_MODULE
#define CHATBOT_RUNTIME_MODULE

#include <array>
#include <filesystem>
#include <string>
#include <vector>

#include "json.h"
#include "utils.h"

struct Question {
    std::string question;
    std::string answer;

    Question(const std::string& question, const std::string& answer)
        : question(question), answer(answer) {}
};

class Runtime {
   private:
    std::filesystem::path path;
    std::vector<Question> questions;

    const std::array<std::string, 4> commands = {"/add", "/update", "/remove", "/exit"};

    bool question_exists(const std::string& question);

    void load();

    void save();

    void print();

    void add(const std::string& question, const std::string& answer);

    void update(const std::string& question, const std::string& answer);

    void remove(const std::string& question);

   public:
    Runtime(const std::string& path) {
        this->path = std::filesystem::path(path);
        this->questions = {};
        this->load();
    }

    void run();
};

#endif
