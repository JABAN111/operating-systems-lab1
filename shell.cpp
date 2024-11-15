#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <vector>
#include <sstream>

void executeCommand(const std::vector<std::string>& args) {
    if (args.empty()) return;


    timeval start{}, end{};
    gettimeofday(&start, nullptr);

    const pid_t pid = vfork();
    if (pid == 0) {
        std::vector<char*> argv;
        for (const auto& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
        argv.push_back(nullptr);
        execvp(argv[0], argv.data());
        perror("execvp failed");
        exit(1);
    }
    if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        gettimeofday(&end, nullptr);

        double elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0;
        elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0;
        std::cout << "Execution time: " << elapsedTime << " ms" << std::endl;
    } else {
        perror("fork failed");
    }
}

std::vector<std::string> parseCommand(const std::string& command) {
    std::istringstream iss(command);
    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }
    return args;
}

int main() {
    while (true) {
        std::cout << "шмель> ";
        std::string command;
        std::getline(std::cin, command);
        if (command == "exit") break;

        std::vector<std::string> args = parseCommand(command);
        executeCommand(args);
    }
    return 0;
}