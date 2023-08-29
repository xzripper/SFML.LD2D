#ifndef SFML_LD2D_LDLOG
#define SFML_LD2D_LDLOG

#include "../SFML\include\SFML\Graphics\Text.hpp"

#include "../SFML\include\SFML\System\String.hpp"

#include "consts.h"

#include <iostream>

class LevelDesignerLog {
public:
    static inline sf::Font font;

    static void initFont() {
        LevelDesignerLog::font.loadFromFile(DEFAULT_WINDOW_FONT);
    }

    static void consoleLog(std::string text) {
        std::cout << "SFML.LD2D: " + text << std::endl;
    }

    static void windowLog(std::string text, sf::RenderWindow &window) {
        sf::Text log(sf::String("SFML.LD2D: " + text), LevelDesignerLog::font, WINDOW_LOG_SIZE);

        log.setPosition(WINDOW_WIDTH / 2, 0);

        window.draw(log);
    }
};

#endif
