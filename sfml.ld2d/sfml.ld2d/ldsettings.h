#ifndef SFML_LD2D_LDSETTINGS
#define SFML_LD2D_LDSETTINGS

#include "../SFML\include\SFML\Graphics\Color.hpp"

#include "consts.h"

class LevelDesignerSettings {
public:
    bool imguiOpen = true;

    sf::Color backgroundColor = {DEFAULT_BACKGROUND_COLOR_R, DEFAULT_BACKGROUND_COLOR_G, DEFAULT_BACKGROUND_COLOR_B};

    int theme = DEFAULT_THEME;

    bool updateView = true;
};

#endif
