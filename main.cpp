//
// Created by JohnScotttt.
// Version 1.0.CPP
//

#include <iostream>
#include <fstream>
#include <string>
#include <yaml-cpp/yaml.h>
#include "argparse.hpp"

//void listEnv();
//void addEnv();
//void removeEnv();
//void enableEnv();
//void disableEnv();


int main(int argc, char **argv) {
    ArgParse arg(argc, argv);
    arg.addArgument("--name",true);
    arg.addArgument("-a","--age",2);
    auto args = arg.parseArgs();
    std::cout << args["name"][0] << std::endl;
    std::cout << args["age"][0] << args["age"][1] << std::endl;
    return 0;
}