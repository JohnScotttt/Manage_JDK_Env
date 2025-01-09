//
// Created by JohnScotttt on 2025/01/08.
//

#include "argparse.hpp"
#include <iostream>
#include <unordered_map>

void ArgParse::setHelp(const std::string& helpInfo) {
    info = helpInfo;
}

void ArgParse::help() {
    std::cout << info << std::endl;
    std::cout.flush();
}

void ArgParse::addArgument(const char *nameFlag, bool isRequired) {
    ArgParse::addArgument(nameFlag, nameFlag, 0, isRequired);
}

void ArgParse::addArgument(const char *nameFlag, int nargs, bool isRequired) {
    ArgParse::addArgument(nameFlag, nameFlag, nargs, isRequired);
}

void ArgParse::addArgument(const char *shortName, const char *longName, bool isRequired){
    ArgParse::addArgument(shortName, longName, 0, isRequired);
}

void ArgParse::addArgument(const char *shortName, const char *longName, int nargs, bool isRequired) {
    std::string ss = shortName;
    std::string sl = longName;
    size_t posS = ss.find_first_not_of('-');
    size_t posL = sl.find_first_not_of('-');

    if (posS == 0 || posL == 0) {
        throw std::runtime_error("Error: Must start with a character '-'");
    }

    if (posS == std::string::npos || posL == std::string::npos) {
        throw std::runtime_error("Error: Invalid argument name");
    }

    std::string sName = ss.substr(posS);
    std::string lName = sl.substr(posL);

    if (nameMap.find(sName) != nameMap.end() || nameMap.find(lName) != nameMap.end()) {
        throw std::runtime_error("Error: Argument name already exists");
    }

    nameMap[sName] = lName;
    argList[lName] = nargs;

    if (isRequired)
        requiredArgs.insert(lName);
}

std::unordered_map<std::string, std::vector<std::string>> ArgParse::parseArgs() {
    std::unordered_map<std::string, std::vector<std::string>> args;

    if (argc == 1) {
        std::cerr << "Error: No arguments provided." << std::endl;
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

                if (argList[name] == 0) {
                    args[name] = std::vector<std::string>{"true"};
                    i++;
                    continue;
                }

                for (int j = 1; j <= argList[name]; j++) {
                    if (i + j < argc)
                        values.emplace_back(argv[i + j]);
                    else {
                        std::cerr << "Error: Not enough arguments for " << arg << std::endl;
                        help();
                        exit(1);
                    }
                }

                args[name] = values;
                i += argList[name] + 1;
            } else {
                std::cerr << "Error: Invalid argument: " << arg << std::endl;
                help();
                exit(1);
            }
        } else{
            std::cerr << "Error: Invalid argument: " << arg << std::endl;
            help();
            exit(1);
        }
    }

    if (!requiredArgs.empty()) {
        std::cerr << "Error: Missing required arguments: ";
        for (const std::string &arg : requiredArgs)
            std::cerr << arg << " ";
        std::cerr << std::endl;
        help();
        exit(1);
    }

    return args;
}