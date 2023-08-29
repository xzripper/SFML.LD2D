#ifndef SFML_LD2D_API_SCENE
#define SFML_LD2D_API_SCENE

#include "../SFML/include/SFML/Graphics/RenderWindow.hpp"

#include "../SFML/include/SFML/Graphics/Color.hpp"

#include "../sfml.ld2d/objbuff.h"

#include "../sfml.ld2d/parser/scene_parser.h"

#include "../sfml.ld2d/objdrawer.h"

#include "../sfml.ld2d/lobject.h"

#include "../sfml.ld2d/ldlog.h"

#include <iostream>

#include <math.h>

class Scene {
public:
    std::string scenePath;

    ObjectsBuffer sceneObjects;

    std::string projectName;

    float sceneBackground[3];

    SceneParser sceneParser;

    Scene(std::string scenePath) {
        try {
            this->sceneParser.parse(scenePath.c_str());
        } catch(std::exception const& exception) {
            LevelDesignerLog::consoleLog("(API) Failed to load scene.");

            return;
        }

        this->scenePath = scenePath;

        this->projectName = this->sceneParser.projectName;

        this->sceneBackground[0] = this->sceneParser.sceneBackground[0];
        this->sceneBackground[1] = this->sceneParser.sceneBackground[1];
        this->sceneBackground[2] = this->sceneParser.sceneBackground[2];

        this->sceneObjects = this->sceneParser.objectsBuffer;
    }

    void drawScene(sf::RenderWindow &window) {
        for(int bufferPosition=0; bufferPosition < this->sceneObjects.getBufferSize(); ++bufferPosition) {
            ObjectDrawer::drawObject(this->sceneObjects.getReferenceBuffer()[bufferPosition], window);
        }
    }

    sf::Color getBackground() {
        return sf::Color(
            round(this->sceneBackground[0] * 255.0),
            round(this->sceneBackground[1] * 255.0),
            round(this->sceneBackground[2] * 255.0),

            255
        );
    }

    LevelObject& getObject(std::string name) {
        return this->sceneObjects.getWithName(name);
    }

    void removeObject(std::string name) {
        this->sceneObjects.removeWithName(name);
    }
};

#endif
