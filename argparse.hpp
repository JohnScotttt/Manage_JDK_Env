//
// Created by JohnScotttt on 2025/01/07.
// Version 1.1
//

#ifndef ARGPARSE_HPP
#define ARGPARSE_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
    (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct ArgBlock {
    std::unordered_map<std::string, std::vector<std::string>> args;
    std::vector<std::string> order;
};

class ArgParse {
private:
    int argc;
    char **argv;
    std::string info = "No help information provided.";
    std::unordered_set<std::string> requiredArgs;
    std::unordered_map<std::string, int> argList;
    std::unordered_map<std::string, std::string> nameMap;

public:
    void setHelp(const std::string& helpInfo);
    void help();

    void addArgument(const char *nameFlag, bool isRequired = false);
    void addArgument(const char *nameFlag, int nargs, bool isRequired = false);
    void addArgument(const char *shortName, const char *longName, bool isRequired = false);
    void addArgument(const char *shortName, const char *longName, int nargs, bool isRequired = false);
    ArgBlock parseArgs();

    ArgParse(int argc, char **argv) {
        this->argc = argc;
        this->argv = argv;
    }

    ~ArgParse() = default;
};
#endif //ARGPARSE_HPP