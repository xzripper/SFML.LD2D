#ifndef SFML_LD2D_LDSCENEVARS
#define SFML_LD2D_LDSCENEVARS

#include <iostream>

#include <map>

class SceneVariables {
public:
    std::map<std::string, std::string> sceneVariables;

    void addSceneVariable(std::string name, std::string content) {
        if(this->sceneVariables.find(name) == this->sceneVariables.end()) {
            this->sceneVariables.insert(std::pair<std::string, std::string>(name, content));
        }
    }

    void removeSceneVariable(std::string name) {
        if(this->sceneVariables.find(name) != this->sceneVariables.end()) {
            this->sceneVariables.erase(this->sceneVariables.find(name));
        }
    }

    std::string getSceneVariable(std::string name) {
        return this->sceneVariables.at(name);
    }

    std::map<std::string, std::string> getSceneVariables() {
        return this->sceneVariables;
    }
};

#endif
