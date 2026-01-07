#ifndef CHATBOT_RUNTIME_MODULE
#define CHATBOT_RUNTIME_MODULE

#include <array>
#include <filesystem>
#include <print>
#include <ranges>
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
    bool is_running;
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

    std::string handle_input();

    void handle_command(const std::string& input);

    void handle_question(const std::string& input);

   public:
    Runtime(const std::string& path) {
        this->is_running = true;
        this->questions = {};
        this->path = std::filesystem::path(path);
        this->load();
    }

    void run();
};

#endif
