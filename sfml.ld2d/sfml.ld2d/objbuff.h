#ifndef SFML_LD2D_OBJBUFF
#define SFML_LD2D_OBJBUFF

#include "lobject.h"

#include <iostream>

#include <vector>

class ObjectsBuffer {
public:
    std::vector<LevelObject> objectsBuffer;

    std::vector<LevelObject> getBuffer() {
        return this->objectsBuffer;
    }

    std::vector<LevelObject> &getReferenceBuffer() {
        return this->objectsBuffer;
    }

    int getBufferSize() {
        return this->objectsBuffer.size();
    }

    void addObject(LevelObject lobject) {
        this->objectsBuffer.push_back(lobject);
    }

    void updateObject(int index, LevelObject updatedObject) {
        this->getReferenceBuffer()[index] = updatedObject;
    }

    LevelObject& getWithName(std::string name) {
        for(int bufferPosition=0; bufferPosition < this->objectsBuffer.size(); ++bufferPosition) {
            if(this->objectsBuffer.at(bufferPosition).lobjectname == name) {
                return this->objectsBuffer.at(bufferPosition);
            }
        }
    }

    LevelObject getWithIndex(int index) {
        return this->objectsBuffer.at(index);
    }

    void removeWithName(std::string name) {
        for(int bufferPosition=0; bufferPosition < this->objectsBuffer.size(); ++bufferPosition) {
            if(this->objectsBuffer.at(bufferPosition).lobjectname == name) {
                this->objectsBuffer.erase(this->objectsBuffer.begin() + bufferPosition);
            }
        }
    }

    void removeWithIndex(int index) {
        this->objectsBuffer.erase(this->objectsBuffer.begin() + index);
    }
};

#endif
