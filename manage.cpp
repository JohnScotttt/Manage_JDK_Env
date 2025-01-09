//
// Created by JohnScotttt on 2025/01/08.
//

#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <yaml-cpp/yaml.h>

void updatePathEnvironmentVariable() {
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_READ | KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        DWORD size = 0;
        RegQueryValueEx(hKey, "Path", nullptr, nullptr, nullptr, &size);
        char* path = new char[size];
        RegQueryValueEx(hKey, "Path", nullptr, nullptr, reinterpret_cast<LPBYTE>(path), &size);

        std::string pathStr = path;
        delete[] path;

        if (pathStr.find("%JAVA_HOME%") == std::string::npos) {
            pathStr += ";%JAVA_HOME%";
            RegSetValueEx(hKey, "Path", 0, REG_EXPAND_SZ, reinterpret_cast<const BYTE*>(pathStr.c_str()), pathStr.size() + 1);
        }

        RegCloseKey(hKey);
    } else {
        std::cerr << "Error: Failed to open the registry key." << std::endl;
    }
}

void listEnv(const YAML::Node& envDict) {
    if (!envDict || envDict.size() == 0) {
        std::cout << "No Java environment found." << std::endl;
        exit(1);
    }

    size_t maxKeyLen = 8;
    size_t maxValueLen = 8;

    for (auto it = envDict.begin(); it != envDict.end(); ++it) {
        if (it->first.as<std::string>().length() > maxKeyLen) {
            maxKeyLen = it->first.as<std::string>().length();
        }

        if (it->second.as<std::string>().length() > maxValueLen) {
            maxValueLen = it->second.as<std::string>().length();
        }
    }

    std::cout << std::left << std::setw(maxKeyLen) << "Env_Name"
              << " | "
              << std::setw(maxValueLen) << "Env_Path" << std::endl;
    std::cout << std::string(maxKeyLen, '-') << "-|-" << std::string(maxValueLen, '-') << std::endl;

    for (auto it = envDict.begin(); it != envDict.end(); ++it) {
        std::cout << std::left << std::setw(maxKeyLen) << it->first.as<std::string>()
                  << " | "
                  << std::setw(maxValueLen) << it->second.as<std::string>() << std::endl;
    }
}

void addEnv(YAML::Node& envDict, const std::string& envName, const std::string& envPath) {
    if (envDict[envName]) {
        std::cerr << "Error: Environment variable already exists." << std::endl;
        exit(1);
    }

    envDict[envName] = envPath;
}

void removeEnv(YAML::Node& javaEnvDict, const std::string& envName) {
    if (!javaEnvDict[envName]) {
        std::cerr << "Error: Environment variable not found." << std::endl;
        exit(1);
    }

    javaEnvDict.remove(envName);
}

void enableEnv(const YAML::Node& envDict, const std::string& envName) {
    if (!envDict[envName]) {
        std::cerr << "Error: Environment variable not found." << std::endl;
        exit(1);
    }

    std::string command = "setx JAVA_HOME \"" + envDict[envName].as<std::string>() + "\\bin\"";

    if (system(command.c_str()) == 0) {
        std::cout << "Java environment " << envName << " enabled successfully." << std::endl;
    } else {
        std::cerr << "Error: Failed to enable Java environment." << std::endl;
    }
}

void disableEnv() {
    if (system("setx JAVA_HOME \"\"") == 0) {
        std::cout << "Java environment is now disabled." << std::endl;
    } else {
        std::cerr << "Error: Failed to disable Java environment." << std::endl;
    }
}