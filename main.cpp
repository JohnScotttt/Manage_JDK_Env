//
// Created by JohnScotttt on 2025/01/09.
// Version 1.1.CPP
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <yaml-cpp/yaml.h>
#include "argparse.hpp"

#define VERSION "1.1.CPP"

void updatePathEnvironmentVariable();
void listEnv(const YAML::Node& envDict);
void addEnv(YAML::Node& envDict, const std::string& envName, const std::string& envPath);
void removeEnv(YAML::Node& envDict, const std::string& envName);
void enableEnv(const YAML::Node& envDict, const std::string& envName);
void disableEnv();

int main(int argc, char **argv) {
    ArgParse arg(argc, argv);
    arg.setHelp("Manage JDK environment\n"
                "Usage: MJE.exe [options]\n"
                "Options:\n"
                "  -l, --list\t\tList all Java environments\n"
                "  -a, --add Env_Name Env_Path\n\t\t\tAdd a new Java environment\n"
                "  -r, --remove Env_Name\n\t\t\tRemove a Java environment\n"
                "  -e, --enable Env_Name\n\t\t\tEnable a Java environment\n"
                "  -d, --disable\t\tDisable all Java environments\n"
                "  -h, --help\t\tShow this help message\n"
                "  -v, --version\t\tShow version information\n");
    arg.addArgument("-l", "--list");
    arg.addArgument("-a", "--add", 2);
    arg.addArgument("-r", "--remove", 1);
    arg.addArgument("-e", "--enable", 1);
    arg.addArgument("-d", "--disable");
    arg.addArgument("-h", "--help");
    arg.addArgument("-v", "--version");

    ArgBlock args = arg.parseArgs();

    updatePathEnvironmentVariable();

    YAML::Node javaEnvDict;
    if (!std::filesystem::exists("env.yml")) {
        std::ofstream file("env.yml");
        std::cerr << "Warning: No env.yml file found, creating a new one." << std::endl;
    }

    try {
        javaEnvDict = YAML::LoadFile("env.yml");
    }
    catch (YAML::Exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    }

    for (const std::string& name:args.order) {
        if (name == "list")
            listEnv(javaEnvDict);
        else if (name == "add")
            addEnv(javaEnvDict, args.args["add"][0], args.args["add"][1]);
        else if (name == "remove")
            removeEnv(javaEnvDict, args.args["remove"][0]);
        else if (name == "enable")
            enableEnv(javaEnvDict, args.args["enable"][0]);
        else if (name == "disable")
            disableEnv();
        else if (name == "help")
            arg.help();
        else if (name == "version")
            std::cout << "MJE core version " << VERSION << std::endl;
    }

    std::ofstream file("env.yml");

    try{
        file << javaEnvDict;
    }
    catch (YAML::Exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    }

    return 0;
}