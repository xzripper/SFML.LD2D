#ifndef SFML_LD2D_PARSER_LDOBJ_PARSER
#define SFML_LD2D_PARSER_LDOBJ_PARSER

#define MIN_LINES 5

#define REGEX "@\\w+\\s+(.*)"

#include "ldp_io.h"

#include "../ldutils.h"

#include "../ldlog.h"

#include <iostream>

#include <vector>

class LDOBJParser {
public:
    std::string objectName;

    std::string spritePath;

    float width;
    float height;

    int rotation;

    // Messy code... TODO: Rewrite.
    int parse(const char* path) {
        std::vector<std::string> lines = LDP_IO::readFileLines(path);

        if(lines.size() < MIN_LINES) {
            LevelDesignerLog::consoleLog("Unable to parse LDOBJ file. Missing minimal 5 lines.");

            return 1;
        }

        std::vector<std::string> tempParsedValue;

        tempParsedValue = LevelDesignerUtilities::rsearch(lines[0], REGEX);

        if(tempParsedValue.size() < 1) {
            LevelDesignerLog::consoleLog("Unable to parse LDOBJ file. Failed to parse ObjectName.");

            return 1;
        } else {
            this->objectName = tempParsedValue[0];
        }

        tempParsedValue = LevelDesignerUtilities::rsearch(lines[1], REGEX);

        if(tempParsedValue.size() < 1) {
            LevelDesignerLog::consoleLog("Unable to parse LDOBJ file. Failed to parse SpritePath.");

            return 1;
        } else {
            this->spritePath = tempParsedValue[0];
        }

        tempParsedValue = LevelDesignerUtilities::rsearch(lines[2], REGEX);

        if(tempParsedValue.size() < 1) {
            LevelDesignerLog::consoleLog("Unable to parse LDOBJ file. Failed to parse Width.");

            return 1;
        } else {
            this->width = std::stof(tempParsedValue[0]);
        }
    
        tempParsedValue = LevelDesignerUtilities::rsearch(lines[3], REGEX);

        if(tempParsedValue.size() < 1) {
            LevelDesignerLog::consoleLog("Unable to parse LDOBJ file. Failed to parse Height.");

            return 1;
        } else {
            this->height = std::stof(tempParsedValue[0]);
        }

        tempParsedValue = LevelDesignerUtilities::rsearch(lines[4], REGEX);

        if(tempParsedValue.size() < 1) {
            LevelDesignerLog::consoleLog("Unable to parse LDOBJ file. Failed to parse Rotation.");

            return 1;
        } else {
            this->rotation = std::stoi(tempParsedValue[0]);
        }

        return 0;
    }
};

#endif
