//
// Created by JohnScotttt on 2025/01/07.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class ArgParse {
private:
    int argc;
    char **argv;
    std::string info = "No help information provided.";
    std::unordered_set<std::string> requiredArgs;
    std::unordered_map<std::string, int> argList;
    std::unordered_map<std::string, std::string> nameMap;
public:
    void setHelp(std::string helpInfo);
    void help();

    void addArgument(const char *nameFlag, bool isRequired = false);
    void addArgument(const char *nameFlag, int nargs = 1, bool isRequired = false);
    void addArgument(const char *shortName, const char *longName, bool isRequired = false);
    void addArgument(const char *shortName, const char *longName, int nargs = 1, bool isRequired = false);

    std::map<std::string, std::vector<std::string>> parseArgs();

    ArgParse(int argc, char **argv) {
        this->argc = argc;
        this->argv = argv;
    }

    ~ArgParse() = default;
};

void ArgParse::setHelp(std::string helpInfo) {
    info = helpInfo;
}

void ArgParse::help() {
    std::cout << info << std::endl;
}

void ArgParse::addArgument(const char *nameFlag, bool isRequired) {
    ArgParse::addArgument(nameFlag, nameFlag, 1, isRequired);
}

void ArgParse::addArgument(const char *nameFlag, int nargs, bool isRequired) {
    ArgParse::addArgument(nameFlag, nameFlag, nargs, isRequired);
}

void ArgParse::addArgument(const char *shortName, const char *longName, bool isRequired){
    ArgParse::addArgument(shortName, longName, 1, isRequired);
}

void ArgParse::addArgument(const char *shortName, const char *longName, int nargs, bool isRequired) {
    std::string ss = shortName;
    std::string sl = longName;
    size_t poss = ss.find_first_not_of("-");
    size_t posl = sl.find_first_not_of("-");
    if (poss == 0 || posl == 0) {
        std::cerr << "Must start with a character '-'" << std::endl;
        exit(1);
    }
    if (poss == std::string::npos || posl == std::string::npos) {
        std::cerr << "Invalid argument name" << std::endl;
        exit(1);
    }
    std::string sName = ss.substr(poss);
    std::string lName = sl.substr(posl);
    if (nameMap.find(sName) != nameMap.end() || nameMap.find(lName) != nameMap.end()) {
        std::cerr << "Argument name already exists" << std::endl;
        exit(1);
    }
    nameMap[sName] = lName;
    argList[lName] = nargs;
    if (isRequired)
        requiredArgs.insert(lName);
}

std::map<std::string, std::vector<std::string>> ArgParse::parseArgs() {
    std::map<std::string, std::vector<std::string>> args;
    if (argc == 1) {
        std::cerr << "No arguments provided." << std::endl;
        help();
        exit(1);
    }
    for (int i = 1; i < argc;) {
        std::string arg = argv[i];
        std::string name;
        if (arg[0] == '-') {
            if (arg[1] == '-')
                name = arg.substr(2);
            else
                name = nameMap[arg.substr(1)];

            if (requiredArgs.find(name) != requiredArgs.end())
                requiredArgs.erase(name);

            if (argList.find(name) != argList.end()) {
                std::vector<std::string> values;
                for (int j = 1; j <= argList[name]; j++) {
                    if (i + j < argc)
                        values.push_back(argv[i + j]);
                    else {
                        std::cerr << "Not enough arguments for " << arg << std::endl;
                        help();
                        exit(1);
                    }
                }
                args[name] = values;
                i += argList[name] + 1;
            } else {
                std::cerr << "Invalid argument: " << arg << std::endl;
                help();
                exit(1);
            }
        } else{
            std::cerr << "Invalid argument: " << arg << std::endl;
            help();
            exit(1);
        }
    }
    if (!requiredArgs.empty()) {
        std::cerr << "Missing required arguments: ";
        for (const std::string &arg : requiredArgs)
            std::cerr << arg << " ";
        std::cerr << std::endl;
        help();
        exit(1);
    }
    return args;
}