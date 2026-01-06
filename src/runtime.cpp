#include "includes/runtime.h"

bool Runtime::question_exists(const std::string& question) {
    for (auto& obj : questions) {
        if (obj.question == question) return true;
    }

    return false;
}

void Runtime::load() {
    if (!std::filesystem::exists(path)) {
        return;
    }

    auto data = json::read(path);

    for (auto& el : data) {
        auto question = Question(el["question"], el["answer"]);
        questions.push_back(question);
    }
}

void Runtime::save() {
    json_t array = json_t::array();

    for (auto& question : questions) {
        json_t question_obj;
        question_obj["question"] = question.question;
        question_obj["answer"] = question.answer;
        array.push_back(question_obj);
    }

    json::write(array, path);
}

void Runtime::print() {
    if (questions.empty()) {
        std::cout << "Empty\n";
        return;
    }

    for (auto& obj : questions) {
        std::cout << obj.question << ": " << obj.answer << "\n";
    }
}

void Runtime::add(const std::string& question, const std::string& answer) {
    if (!question_exists(question)) {
        auto obj = Question(question, answer);
        questions.push_back(obj);
        save();
        return;
    }

    int choice;
    std::cout << "Question already exists, do you want to update this one?\n";
    std::cout << "1. Yes\n2. No\n";
    std::cin >> choice;

    if (choice == 1) update(question, answer);
}

void Runtime::update(const std::string& question, const std::string& answer) {
    if (!question_exists(question)) return;

    for (auto& obj : questions) {
        if (obj.question == question) {
            obj.answer = answer;
            save();
            return;
        }
    }
}

void Runtime::remove(const std::string& question) {
    if (!question_exists(question)) return;

    for (int i = 0; i < questions.size(); i++) {
        if (questions[i].question == question) {
            questions.erase(questions.begin() + i);
            save();
            return;
        }
    }
}

void Runtime::run() {
    bool is_running = true;

    while (is_running) {
        std::string input;
        std::cin >> input;
        utils::trim(input);

        if (input == "/exit") {
            is_running = false;
        }
    }
}
