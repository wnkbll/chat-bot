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
        std::printf("Bot: Question \"%s\" successfully added \n\n", question.c_str());
        return;
    }

    std::cout << "Bot: Question already exists, do you want to update this one?\n";
    std::cout << "1. Yes\n2. No\n";
    std::string input;
    std::getline(std::cin, input);
    

    int choice = 0;

    try {
        choice = std::stoi(input);
    } catch (const std::invalid_argument& e) {
        utils::replace_line(1, "You: are stupid");
        return;
    }

    if (choice == 1) {
        utils::replace_line(1, "You: Yes");
        update(question, answer);
    } else {
        utils::replace_line(1, "You: No");
    }
}

void Runtime::update(const std::string& question, const std::string& answer) {
    if (!question_exists(question)) {
        std::printf("Bot: Question \"%s\" does not exist\n\n", question.c_str());
        return;
    }

    for (auto& obj : questions) {
        if (obj.question == question) {
            obj.answer = answer;
            save();
            std::printf("Bot: Question \"%s\" successfully updated \n\n",
                        question.c_str());
            return;
        }
    }
}

void Runtime::remove(const std::string& question) {
    if (!question_exists(question)) {
        std::printf("Bot: Question \"%s\" does not exist\n\n", question.c_str());
        return;
    }

    for (int i = 0; i < questions.size(); i++) {
        if (questions[i].question == question) {
            questions.erase(questions.begin() + i);
            save();
            std::printf("Bot: Question \"%s\" successfully removed\n\n",
                        question.c_str());
            return;
        }
    }
}

std::string Runtime::handle_input() {
    std::string input;
    std::getline(std::cin, input);
    utils::trim(input);
    utils::replace_line(1, std::format("You: {}", input));

    return input;
}

void Runtime::handle_command(const std::string& input) {
    int pos = input.find(" ");
    std::string command = input.substr(0, pos);
    std::string args;

    if (pos != std::string::npos) {
        args = input.substr(pos + 1);
    }

    utils::trim(command);
    utils::trim(args);

    if (!std::ranges::contains(commands, command)) {
        std::printf("Bot: Command %s does not exist\n\n", command.c_str());
        return;
    }

    if (command == "/add") {
        std::string question, answer;
        bool is_valid = utils::parse_args(args, question, answer);

        if (!is_valid) {
            std::printf("Bot: Command %s requires 2 args, included in \"\n\n",
                        command.c_str());
            return;
        }

        add(question, answer);
        return;
    }

    if (command == "/update") {
        std::string question, answer;
        bool is_valid = utils::parse_args(args, question, answer);

        if (!is_valid) {
            std::printf("Bot: Command %s requires 2 args, included in \"\n\n",
                        command.c_str());
            return;
        }

        update(question, answer);
        return;
    }

    if (command == "/remove") {
        std::string question;
        bool is_valid = utils::parse_args(args, question);

        if (!is_valid) {
            std::printf("Bot: Command /remove requires 1 arg, included in \"\n\n");
            return;
        }

        remove(question);
        return;
    }

    if (command == "/list") {
        if (questions.empty()) {
            std::printf("Bot: List is empty\n\n");
            return;
        }

        std::printf("Bot: Here is your list of questions and answers\n\n");
        for (const auto& question : questions) {
            std::printf("Q: %s\n", question.question.c_str());
            std::printf("A: %s\n\n", question.answer.c_str());
        }
        return;
    }

    if (command == "/exit") {
        is_running = false;
        std::printf("Bot: Bye bye!\n");
        return;
    }
}

void Runtime::handle_question(const std::string& input) {
    if (!question_exists(input)) {
        std::printf("Bot: I don't know\n\n");
        return;
    }

    for (auto& question : questions) {
        if (question.question == input) {
            std::printf("Bot: %s\n\n", question.answer.c_str());
            return;
        }
    }
}

void Runtime::run() {
    std::cout << "Welcome to chat!\n\n";

    while (is_running) {
        std::string input = handle_input();

        if (input.starts_with("/")) {
            handle_command(input);
        } else {
            handle_question(input);
        }
    }
}
