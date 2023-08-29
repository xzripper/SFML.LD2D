#ifndef SFML_LD2D_LOBJECT
#define SFML_LD2D_LOBJECT

#include "../SFML\include\SFML\Graphics\Color.hpp"

#include "lobjectype.h"

#include "shadertypes.h"

#include <optional>

#include <iostream>

class LevelObject {
public:
    LevelObjectType lobjecttype;

    std::string lobjectname;

    int objectWidth, objectHeight;
    int objectX, objectY;

    bool hidden = false;

    bool sizeNormalized = false;

    std::optional<float> objectSWidth, objectSHeight;

    std::optional<int> objectRotation;

    std::optional<int> objectRadius;

    std::optional<std::string> text;

    std::optional<std::string> font;

    std::optional<int> fontSize;

    std::optional<bool> bold;
    std::optional<bool> italic;
    std::optional<bool> underline;

    std::optional<std::string> spritePath;

    std::optional<int> shader = ShaderTypes::NO_SHADERS;

    std::optional<float> shader_pixelThreshold = 0.01f;

    std::optional<float> shader_blurRadius = 0.01f;

    sf::Color objectColor;
};

#endif
