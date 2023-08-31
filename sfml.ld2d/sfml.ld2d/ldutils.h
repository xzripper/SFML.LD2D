#ifndef SFML_LD2D_LDUTILS
#define SFML_LD2D_LDUTILS

#include "../imgui\include\imgui.h"

#include "../SFML\include\SFML\Graphics.hpp"

#include "themes.h"

#include "consts.h"

#include <math.h>

#include <iostream>

#include <vector>

#include <regex>

#include <string>

#include <initializer_list>

#include <filesystem>

class LevelDesignerUtilities {
public:
    static inline bool release = true;

    static int glColorToSFMLColor(float c) {
        return round(c * 255.0);
    }

    static int SFMLColorToGl(int c) {
        return c / 255.0;
    }

    static sf::Color glColorArrayToSFMLColor(float rgb[]) {
        return sf::Color {
            LevelDesignerUtilities::glColorToSFMLColor(rgb[0]),
            LevelDesignerUtilities::glColorToSFMLColor(rgb[1]),
            LevelDesignerUtilities::glColorToSFMLColor(rgb[2])
        };
    }

    static sf::Color glColorArrayToSFMLColorA(float rgba[]) {
        return sf::Color {
            LevelDesignerUtilities::glColorToSFMLColor(rgba[0]),
            LevelDesignerUtilities::glColorToSFMLColor(rgba[1]),
            LevelDesignerUtilities::glColorToSFMLColor(rgba[2]),
            LevelDesignerUtilities::glColorToSFMLColor(rgba[3])
        };
    }

    static const char *validateName(const char *objectName, const char *objectNameHint) {
        return strlen(objectName) == 0 ? objectNameHint : objectName;
    }

    static void updateTheme(int theme) {
        if(theme == THEME_IMGUI_DARK) {
            ImGui::StyleColorsDark();
        } else if(theme == THEME_IMGUI_LIGHT) {
            ImGui::StyleColorsLight();
        } else if(theme == THEME_DARK_RED) {
            LevelDesignerThemes::DarkRed();
        } else if(theme == THEME_DARK_GREEN) {
            LevelDesignerThemes::DarkGreen();
        } else if(theme == THEME_DEEP_DARK) {
            LevelDesignerThemes::DeepDark();
        } else if(theme == THEME_FUTURE_DARK) {
            LevelDesignerThemes::FutureDark();
        } else if(theme == THEME_SOFT_CHERRY) {
            LevelDesignerThemes::SoftCherry();
        } else if(theme == THEME_SFML_LD2D) {
            LevelDesignerThemes::SFMLLD2D();
        }
    }

    static bool intersects(sf::Vector2<int> position1, sf::Vector2<int> size1, sf::Vector2<int> position2, sf::Vector2<int> size2) {
        sf::IntRect first(position1, size1);
        sf::IntRect second(position2, size2);

        return first.intersects(second);
    }

    static bool mIntersects(sf::Vector2<int> size, sf::Vector2<int> position, ImVec2 mousePosition) {
        sf::IntRect object(position, size);
        sf::IntRect mouse(mousePosition.x, mousePosition.y, DEFAULT_CURSOR_WIDTH, DEFAULT_CURSOR_HEIGHT);

        return object.intersects(mouse);
    }

    static std::string joinVector(const char* delimiter, std::vector<std::string> vector) {
        std::string joined = "";

        for(std::string element : vector) {
            if(element != vector.at(vector.size() - 1)) {
                element.append(delimiter);
            }

            joined += element;
        }

        return joined;
    }

    static std::vector<std::string> rsearch(std::string in, const char* _pattern) {
        std::vector<std::string> out;

        std::regex pattern(_pattern);

        std::smatch match;

        if(std::regex_search(in, match, pattern)) {
            if(match.size() == 1) {
                out.push_back(match.str(1));
            } else if(match.size() >= 2) {
                for(int found=1; found < match.size(); ++found) {
                    out.push_back(match.str(found));
                }
            }
        }

        return out;
    }

    // https://stackoverflow.com/a/46931770/19845821
    static std::vector<std::string> split(std::string in, std::string delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> res;

        while ((pos_end = in.find(delimiter, pos_start)) != std::string::npos) {
            token = in.substr (pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back (token);
        }

        res.push_back (in.substr (pos_start));
        return res;
    }

    static std::string format(std::string string, std::initializer_list<std::string> _format) {
        std::string formatted = string;
    
        for(int position = 0; position < _format.size(); ++position) {
            formatted = std::regex_replace(formatted, std::regex("#" + std::to_string(position) + "\\b"), *(_format.begin() + position));
        }
    
        return formatted;
    }

    static std::string getAbsolutePath(std::string path) {
        return (LevelDesignerUtilities::release ? "sfml.ld2d" : "") + std::filesystem::absolute(std::filesystem::path(path)).string();
    }

    static bool exists(std::string path) {
        return std::filesystem::exists(std::filesystem::path(path));
    }
};

#endif
