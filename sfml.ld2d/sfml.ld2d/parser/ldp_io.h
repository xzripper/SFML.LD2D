#ifndef SFML_LD2D_PARSER_IO
#define SFML_LD2D_PARSER_IO

#include "../ldutils.h"

#include <iostream>

#include <fstream>

#include <vector>

class LDP_IO {
public:
    static std::vector<std::string> readFileLines(const char* filePath) {
        std::vector<std::string> content;

        std::string line;

        std::ifstream file(filePath);

        if(file.is_open()) {
            while(std::getline(file, line)) {
                content.push_back(line);
            }
        }

        file.close();

        return content;
    }

    static std::string readFile(const char* filePath) {
        return LevelDesignerUtilities::joinVector("\n", LDP_IO::readFileLines(filePath));
    }

    static void writeFile(const char* filePath, std::string in, bool newline) {
        std::ofstream file(filePath);

        file << in + (newline ? "\n" : "");

        file.close();
    }
};

#endif
