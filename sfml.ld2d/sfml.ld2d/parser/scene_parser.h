#ifndef SFML_LD2D_PARSER_SCENE_PARSER
#define SFML_LD2D_PARSER_SCENE_PARSER

#define REGEX_S "<([^>]+)>"

#define DELIMITER ":"

#include "ldp_io.h"

#include "../consts.h"

#include "../lobject.h"

#include "../lobjectype.h"

#include "../shadertypes.h"

#include "../objbuff.h"

#include "../ldutils.h"

#include "../ldlog.h"

#include <iostream>

#include <vector>

#include <string>

class SceneParser {
public:
    ObjectsBuffer objectsBuffer;

    std::vector<std::string> objectsToLoad;

    std::string projectName;

    std::vector<float> sceneBackground;

    int parse(const char* path) {
        std::vector<std::string> lines = LDP_IO::readFileLines(path);

        if(lines.size() <= 0) {
            LevelDesignerLog::consoleLog("Failed to parse scene " + std::string(path) + ".");

            return 1;
        }

        for(int line=0; line < lines.size(); ++line) {
            std::vector<std::string> parsedValues = LevelDesignerUtilities::split(LevelDesignerUtilities::rsearch(lines[line], REGEX_S)[0], DELIMITER);

            std::string type = parsedValues[0];

            if(type == "SCENE") {
                this->projectName = parsedValues[1];

                this->sceneBackground = {
                    std::stof(parsedValues[2]),
                    std::stof(parsedValues[3]),
                    std::stof(parsedValues[4]),
                };
            } else if(type == "RECTANGLE") {
                float rgba[] = {std::stof(parsedValues[8]), std::stof(parsedValues[9]), std::stof(parsedValues[10]), std::stof(parsedValues[11])};

                LevelObject object;

                object.lobjecttype = LevelObjectType::RECTANGLE;

                object.lobjectname = parsedValues[1];

                object.objectWidth = std::stoi(parsedValues[2]);
                object.objectHeight = std::stoi(parsedValues[3]);

                object.objectX = std::stoi(parsedValues[4]);
                object.objectY = std::stoi(parsedValues[5]);

                object.objectRotation = std::stoi(parsedValues[6]);

                object.hidden = parsedValues[7] == "true";

                object.objectColor = LevelDesignerUtilities::glColorArrayToSFMLColorA(rgba);

                this->objectsBuffer.addObject(object);
            } else if(type == "CIRCLE") {
                float rgba[] = {std::stof(parsedValues[9]), std::stof(parsedValues[10]), std::stof(parsedValues[11]), std::stof(parsedValues[12])};

                LevelObject object;

                object.lobjecttype = LevelObjectType::CIRCLE;

                object.lobjectname = parsedValues[1];

                object.objectWidth = std::stoi(parsedValues[2]);
                object.objectHeight = std::stoi(parsedValues[3]);

                object.objectX = std::stoi(parsedValues[4]);
                object.objectY = std::stoi(parsedValues[5]);

                object.objectRadius = std::stoi(parsedValues[6]);

                object.objectRotation = std::stoi(parsedValues[7]);

                object.hidden = parsedValues[8] == "true";

                object.objectColor = LevelDesignerUtilities::glColorArrayToSFMLColorA(rgba);

                this->objectsBuffer.addObject(object);
            } else if(type == "TEXT") {
                float rgba[] = {std::stof(parsedValues[12]), std::stof(parsedValues[13]), std::stof(parsedValues[14]), 1.0f};

                LevelObject object;

                object.lobjecttype = LevelObjectType::TEXT;

                object.lobjectname = parsedValues[1];

                object.text = parsedValues[2];

                object.font = parsedValues[3].replace(parsedValues[3].find("builtin@"), 8, BUILTIN);

                object.fontSize = std::stoi(parsedValues[4]);

                object.bold = parsedValues[5] == "true";
                object.italic = parsedValues[6] == "true";
                object.underline = parsedValues[7] == "true";

                object.objectX = std::stoi(parsedValues[8]);
                object.objectY = std::stoi(parsedValues[9]);

                object.objectRotation = std::stoi(parsedValues[10]);

                object.hidden = parsedValues[11] == "true";

                object.objectColor = LevelDesignerUtilities::glColorArrayToSFMLColorA(rgba);

                this->objectsBuffer.addObject(object);
            } else if(type == "SPRITE") {
                LevelObject object;

                object.lobjecttype = LevelObjectType::SPRITE;

                object.lobjectname = parsedValues[1];

                object.objectSWidth = std::stof(parsedValues[2]);
                object.objectSHeight = std::stof(parsedValues[3]);

                object.objectX = std::stoi(parsedValues[4]);
                object.objectY = std::stoi(parsedValues[5]);

                object.spritePath = parsedValues[6];

                object.objectRotation = std::stoi(parsedValues[7]);

                if(parsedValues[8] == "PIXELATE") {
                    object.shader = ShaderTypes::PIXELATE;

                    object.shader_pixelThreshold = std::stof(parsedValues[9]);
                } else if(parsedValues[8] == "BLUR") {
                    object.shader = ShaderTypes::BLUR;

                    object.shader_blurRadius = std::stof(parsedValues[9]);
                } else if(parsedValues[8] == "NO_SHADERS") {
                    object.shader == ShaderTypes::NO_SHADERS;
                }

                object.hidden = parsedValues[10] == "true";

                this->objectsBuffer.addObject(object);
            } else if(type == "LDOBJ") {
                this->objectsToLoad.push_back(parsedValues[1]);
            }
        }

        LevelDesignerLog::consoleLog("Scene " + std::string(path) + " parsed.");

        return 0;
    }

    void write(const char* path, std::string projectName, float rgb[], ObjectsBuffer objectsBuffer, std::vector<std::string> objectsToLoad) {
        std::string out;

        out += LevelDesignerUtilities::format("<SCENE:#0:#1:#2:#3>\n", { projectName, std::to_string(rgb[0]), std::to_string(rgb[1]), std::to_string(rgb[2]) });

        for(int posObjectsBuffer=0; posObjectsBuffer < objectsBuffer.getBufferSize(); ++posObjectsBuffer) {
            LevelObject object = objectsBuffer.getWithIndex(posObjectsBuffer);

            if(object.lobjecttype == LevelObjectType::RECTANGLE) {
                out += LevelDesignerUtilities::format(
                    "<RECTANGLE:#0:#1:#2:#3:#4:#5:#6:#7:#8:#9:#10>\n",

                    {
                        object.lobjectname,

                        std::to_string(object.objectWidth),
                        std::to_string(object.objectHeight),

                        std::to_string(object.objectX),
                        std::to_string(object.objectY),

                        std::to_string(object.objectRotation.value_or(0)),

                        object.hidden ? "true" : "false",

                        std::to_string(LevelDesignerUtilities::SFMLColorToGl(object.objectColor.r)),
                        std::to_string(LevelDesignerUtilities::SFMLColorToGl(object.objectColor.g)),
                        std::to_string(LevelDesignerUtilities::SFMLColorToGl(object.objectColor.b)),
                        std::to_string(LevelDesignerUtilities::SFMLColorToGl(object.objectColor.a))
                    }
                );
            } else if(object.lobjecttype == LevelObjectType::CIRCLE) {
                out += LevelDesignerUtilities::format(
                    "<CIRCLE:#0:#1:#2:#3:#4:#5:#6:#7:#8:#9:#10:#11>\n",

                    {
                        object.lobjectname,

                        std::to_string(object.objectWidth),
                        std::to_string(object.objectHeight),

                        std::to_string(object.objectX),
                        std::to_string(object.objectY),

                        std::to_string(object.objectRadius.value_or(0)),

                        std::to_string(object.objectRotation.value_or(0)),

                        object.hidden ? "true" : "false",

                        std::to_string(LevelDesignerUtilities::SFMLColorToGl(object.objectColor.r)),
                        std::to_string(LevelDesignerUtilities::SFMLColorToGl(object.objectColor.g)),
                        std::to_string(LevelDesignerUtilities::SFMLColorToGl(object.objectColor.b)),
                        std::to_string(LevelDesignerUtilities::SFMLColorToGl(object.objectColor.a)),
                    }
                );
            } else if(object.lobjecttype == LevelObjectType::TEXT) {
                out += LevelDesignerUtilities::format(
                    "<TEXT:#0:#1:#2:#3:#4:#5:#6:#7:#8:#9:#10:#11:#12:#13>\n",

                    {
                        object.lobjectname,

                        object.text.value_or(DEFAULT_OBJECTS_TEXT),

                        object.font.value_or(DEFAULT_WINDOW_FONT).replace(sizeof(BUILTIN) - 3, 2, "@"),

                        std::to_string(object.fontSize.value_or(0)),

                        object.bold.value() ? "true" : "false",
                        object.italic.value() ? "true" : "false",
                        object.underline.value() ? "true" : "false",

                        std::to_string(object.objectX),
                        std::to_string(object.objectY),

                        std::to_string(object.objectRotation.value_or(0)),

                        object.hidden ? "true" : "false",

                        std::to_string(LevelDesignerUtilities::SFMLColorToGl(object.objectColor.r)),
                        std::to_string(LevelDesignerUtilities::SFMLColorToGl(object.objectColor.g)),
                        std::to_string(LevelDesignerUtilities::SFMLColorToGl(object.objectColor.b))
                    }
                );
            } else if(object.lobjecttype == LevelObjectType::SPRITE) {
                std::string shader;

                float shaderParameter;

                if(object.shader == ShaderTypes::PIXELATE) {
                    shader = "PIXELATE";

                    shaderParameter = object.shader_pixelThreshold.value_or(0);
                } else if(object.shader == ShaderTypes::BLUR) {
                    shader = "BLUR";

                    shaderParameter = object.shader_blurRadius.value_or(0);
                } else if(object.shader == ShaderTypes::NO_SHADERS) {
                    shader = "NO_SHADERS";
                }

                out += LevelDesignerUtilities::format(
                    "<SPRITE:#0:#1:#2:#3:#4:#5:#6:#7:#8:#9>\n",

                    {
                        object.lobjectname,

                        std::to_string(object.objectSWidth.value()),
                        std::to_string(object.objectSHeight.value()),

                        std::to_string(object.objectX),
                        std::to_string(object.objectY),

                        object.spritePath.value_or(MISSING_SPRITE_PATH),

                        std::to_string(object.objectRotation.value_or(0)),

                        shader,

                        std::to_string(shaderParameter),

                        object.hidden ? "true" : "false"
                    }
                );
            } else if(object.lobjecttype == LevelObjectType::EMPTY) {}
        }

        for(int posObjToLoad=0; posObjToLoad < objectsToLoad.size(); ++posObjToLoad) {
            out += LevelDesignerUtilities::format(
                "<LDOBJ:#0>\n",

                {objectsToLoad[posObjToLoad]}
            );
        }

        LDP_IO::writeFile(path, out, false);
    }
};

#endif
