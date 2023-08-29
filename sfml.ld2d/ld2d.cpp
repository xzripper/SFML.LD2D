// Includes.
#include "SFML\include\SFML\Graphics.hpp"

#include "imgui\include\imgui.h"
#include "imgui\include\imgui-SFML.h"

#include "sfml.ld2d\consts.h"

#include "sfml.ld2d\ldsettings.h"
#include "sfml.ld2d\ldutils.h"

#include "sfml.ld2d\objdrawer.h"

#include "sfml.ld2d\objbuff.h"

#include "sfml.ld2d\lobject.h"

#include "sfml.ld2d\lobjectype.h"

#include "sfml.ld2d\objeditorui.h"

#include "sfml.ld2d\ldscenevars.h"

#include "sfml.ld2d\parser\ldobj_parser.h"

#include "sfml.ld2d\parser\scene_parser.h"

#include "sfml.ld2d\ldlog.h"

#include "sfml.ld2d\themes.h"

#include <iostream>

#include <string>

#include <vector>

// #include <map>

// ImGui Log Message Function.
void imgw_log(const char* text, ImVec4 color) { ImGui::TextColored(color, text); }

// Main function.
int main() {
    // Window initialization.
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    // Set window icon.
    sf::Image windowIcon;

    windowIcon.loadFromFile("icon.png");

    window.setIcon(windowIcon.getSize().x, windowIcon.getSize().y, windowIcon.getPixelsPtr());

    // Initialize ImGui window.
    ImGui::SFML::Init(window);
 
    // Level designer settings.
    LevelDesignerSettings ldsettings;

    // Level designer objects buffer.
    ObjectsBuffer objectsBuffer;

    // Level designer loaded objects buffer.
    ObjectsBuffer loadedObjectsBuffer;

    // Scene variables.
    SceneVariables sceneVariables;

    // Objects parser.
    LDOBJParser ldobjParser;

    // Scene parser.
    SceneParser sceneParser;

    // Various variables.
    static std::string projectName = UNTITLED;

    static float backgroundColor[] = {0.0f, 0.0f, 0.0f};

    static char _objectNameHint[28];
    static char _objectName[28];

    static int selectedObjectIndex = -1;

    LevelObject selectedObject;

    bool failedToLoadLDOBJ = false;

    bool failedToLoadScene = false;

    bool corruptedScene = false;

    bool sFailedToLoadLDOBJ = false;

    std::vector<std::string> objectsToLoad;

    int imported = 0;
    int exported = 0;

    // IB Textures Vector.
    std::vector<sf::Texture> ibTexturesVector;

    // Application clock.
    sf::Clock clock;

    // Dynamic main menu height.
    int dynHeightMain = WINDOW_HEIGHT;

    // Is Drag&Drop active?
    bool dragndropping = false;

    // Initialize log font.
    LevelDesignerLog::initFont();

    // Register "special" object in the buffer.
    LevelObject specObj;

    specObj.lobjecttype = LevelObjectType::EMPTY;

    specObj.lobjectname = "SFML.LD2D Entity.";

    specObj.objectX = 0;
    specObj.objectY = 0;

    specObj.objectWidth = 0;
    specObj.objectHeight = 0;

    objectsBuffer.addObject(specObj);

    // Preview texture allocations.
    sf::Texture* texture = new sf::Texture();

    // Main window loop.
    while(window.isOpen()) {
        int ibTexturePos = 0;

        // Update ImGui window.
        ImGui::SFML::Update(window, clock.restart());

        // Begin ImGui window.
        ImGui::Begin(IMGUI_WINDOW_TITLE, &ldsettings.imguiOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        // Set ImGui window properties.
        ImGui::SetWindowSize(ImVec2 {IMGUI_WINDOW_WIDTH, dynHeightMain});
        ImGui::SetWindowPos(ImVec2 {IMGUI_WINDOW_X, IMGUI_WINDOW_Y});

        // Update theme.
        LevelDesignerUtilities::updateTheme(ldsettings.theme);

        // Event.
        sf::Event event;

        // Handle events.
        while(window.pollEvent(event)) {
            // Process event in ImGui.
            ImGui::SFML::ProcessEvent(event);

            // Handle window closing and window resize.
            if(event.type == sf::Event::Closed) {
                window.close();
            } else if(event.type == sf::Event::Resized) {
                dynHeightMain = event.size.height;

                ObjectEditorUI::dynHeight = event.size.height;

                ObjectEditorUI::dynX = event.size.width - OBJECT_EDITOR_UI_WIDTH;

                if(ldsettings.updateView) {
                    window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                }
            }
        }

        // Hierarchy of objects.
        ImGui::Text(objectsBuffer.getBufferSize() >= 2 ? "Hierarchy." : "Hierarchy (Empty).");

        for(int _selectedObjectIndex=0; _selectedObjectIndex < objectsBuffer.getBufferSize(); ++_selectedObjectIndex) {
            LevelObject object = objectsBuffer.getWithIndex(_selectedObjectIndex);

            if(object.lobjecttype != LevelObjectType::EMPTY) {
                if(ImGui::Selectable(((std::to_string(_selectedObjectIndex) + ". ") + object.lobjectname).c_str())) {
                    selectedObjectIndex = _selectedObjectIndex;
    
                    selectedObject = object;
                }
            }
        }

        if(selectedObjectIndex != -1) {
            ImGui::Text(("Selected: " + selectedObject.lobjectname).c_str());
        }

        // Hierarchy: Interactions with selected objects.
        if(ImGui::Button("Delete selected object.")) {
            if(selectedObjectIndex != -1) {
                objectsBuffer.removeWithIndex(selectedObjectIndex);

                selectedObjectIndex = -1;
            }
        }

        if(ImGui::Button("Copy selected object.")) {
            if(selectedObjectIndex != -1) {
                LevelObject objectToCopy = selectedObject;

                objectToCopy.lobjectname = "Copy of " + objectToCopy.lobjectname;

                objectsBuffer.addObject(objectToCopy);
            }
        }

        ImGui::NewLine();

        // General header.
        if(ImGui::CollapsingHeader("General.")) {
            // Update project name.
            ImGui::InputText("Project name.", &projectName);

            ImGui::NewLine();

            // Update view?
            ImGui::Checkbox("Update view?", &ldsettings.updateView);

            ImGui::NewLine();

            // Change theme.
            ImGui::RadioButton("Dark theme.", &ldsettings.theme, 0);
            ImGui::RadioButton("Light theme.", &ldsettings.theme, 1);

            ImGui::RadioButton("Dark Red theme.", &ldsettings.theme, 2);
            ImGui::RadioButton("Dark Green theme.", &ldsettings.theme, 3);

            ImGui::RadioButton("Deep Dark theme.", &ldsettings.theme, 4);

            ImGui::RadioButton("Future Dark theme.", &ldsettings.theme, 5);

            ImGui::RadioButton("Soft Cherry theme.", &ldsettings.theme, 6);

            ImGui::RadioButton("SFML.LD2D theme.", &ldsettings.theme, 7);

            ImGui::Text("Theme: %d", ldsettings.theme);

            ImGui::NewLine();

            // Change background color.
            ImGui::ColorEdit3("Background color.", backgroundColor);

            // Separate header from other headers.
            ImGui::NewLine();
        }

        // Scene header. (TODO: Doesn't works).
        // if(ImGui::CollapsingHeader("Scene.")) {
        //     for(sceneVariableIterator = sceneVariables.getSceneVariables().begin(); sceneVariableIterator != sceneVariables.getSceneVariables().end(); ++sceneVariableIterator) {
        //         static std::string name = sceneVariableIterator->first;

        //         static std::string content = sceneVariableIterator->second;

        //         ImGui::InputText("##scene_variable_name_input", &name);

        //         ImGui::SameLine();

        //         ImGui::InputText("##scene_variable_content_input", &content);
        //     }
        // }
        
        // Objects header.
        if(ImGui::CollapsingHeader("Objects.")) {
            // General settings for objects.
            strcpy(_objectNameHint, ("Object " + std::to_string(objectsBuffer.getBufferSize())).c_str());

            ImGui::InputTextWithHint("Object name.", _objectNameHint, _objectName, IM_ARRAYSIZE(_objectName));

            const char* objectName = LevelDesignerUtilities::validateName(_objectName, _objectNameHint);

            // Shapes.
            if(ImGui::CollapsingHeader("Shapes.")) {
                // Rectangle.
                if(ImGui::CollapsingHeader("Rectangle.")) {
                    // Rectangle properties.
                    static int rectangleWidth, rectangleHeight, rectangleX, rectangleY;

                    static float rectangleColor[4] = {DEFAULT_OBJECTS_R_gl, DEFAULT_OBJECTS_G_gl, DEFAULT_OBJECTS_B_gl, DEFAULT_OBJECTS_A_gl};

                    ImGui::InputInt("Width.##rect", &rectangleWidth); ImGui::InputInt("Height.##rect", &rectangleHeight);
                    ImGui::InputInt("X.##rect", &rectangleX); ImGui::InputInt("Y.##rect", &rectangleY);

                    ImGui::ColorEdit4("Color.##rect", rectangleColor);

                    // Create rectangle.
                    if(ImGui::Button("Create rectangle.")) {
                        LevelObject rectangle;

                        rectangle.lobjecttype = LevelObjectType::RECTANGLE;

                        rectangle.lobjectname = objectName;

                        rectangle.objectWidth = rectangleWidth;
                        rectangle.objectHeight = rectangleHeight;

                        rectangle.objectX = rectangleX;
                        rectangle.objectY = rectangleY;

                        rectangle.objectColor = LevelDesignerUtilities::glColorArrayToSFMLColorA(rectangleColor);

                        objectsBuffer.addObject(rectangle);

                        LevelDesignerLog::consoleLog("Rectangle '" + std::string(objectName) + "' created.");
                    }
                }

                // Circle.
                if(ImGui::CollapsingHeader("Circle.")) {
                    // Circle properties.
                    static int circleWidth, circleHeight, circleX, circleY, circleRadius;

                    static float circleColor[4] = {DEFAULT_OBJECTS_R_gl, DEFAULT_OBJECTS_G_gl, DEFAULT_OBJECTS_B_gl, DEFAULT_OBJECTS_A_gl};

                    ImGui::InputInt("Width.##circle", &circleWidth); ImGui::InputInt("Height.##circle", &circleHeight);
                    ImGui::InputInt("X.##circle", &circleX); ImGui::InputInt("Y.##circle", &circleY);

                    ImGui::InputInt("Radius.##circle", &circleRadius);

                    ImGui::ColorEdit4("Color.##circle", circleColor);

                    // Checks.
                    if(circleRadius <= 0) { imgw_log("Circle radius should be more than 0.", ImVec4 {1.0f, 0.0, 0.0, 1.0f}); }

                    // Create circle.
                    if(ImGui::Button("Create circle.")) {
                        if(circleRadius >= 1) {
                            LevelObject circle;

                            circle.lobjecttype = LevelObjectType::CIRCLE;
    
                            circle.lobjectname = objectName;
    
                            circle.objectWidth = circleWidth;
                            circle.objectHeight = circleHeight;
    
                            circle.objectX = circleX;
                            circle.objectY = circleY;
    
                            circle.objectRadius = circleRadius;
    
                            circle.objectColor = LevelDesignerUtilities::glColorArrayToSFMLColorA(circleColor);
    
                            objectsBuffer.addObject(circle);
    
                            LevelDesignerLog::consoleLog("Circle '" + std::string(objectName) + "' created.");
                        }
                    }
                }

                // Separate header from other headers.
                ImGui::NewLine();
            }

            // Text.
            if(ImGui::CollapsingHeader("Text.")) {
                // Text properties.
                static char text[MAX_OBJECTS_TEXT_SIZE] = DEFAULT_OBJECTS_TEXT;
                static char font[MAX_OBJECTS_TEXT_SIZE / 2] = DEFAULT_OBJECTS_FONT;

                static int fontSize = WINDOW_LOG_SIZE;

                static int textX, textY;

                static float textColor[3] = {DEFAULT_OBJECTS_R_gl, DEFAULT_OBJECTS_G_gl, DEFAULT_OBJECTS_B_gl};

                static bool bold, italic, underline = false;

                ImGui::InputText("Text.##ttext", text, IM_ARRAYSIZE(text));

                ImGui::InputText("Font.##ttext", font, IM_ARRAYSIZE(font));

                ImGui::InputInt("Font size.##ttext", &fontSize);

                ImGui::InputInt("Text X.##ttext", &textX);
                ImGui::InputInt("Text Y.##ttext", &textY);

                ImGui::ColorEdit3("Color.##ttext", textColor);

                ImGui::Checkbox("Bold.", &bold); ImGui::SameLine(); ImGui::Checkbox("Italic.", &italic); ImGui::SameLine(); ImGui::Checkbox("Underline.", &underline);

                // Create text.
                if(ImGui::Button("Create text.")) {
                    LevelObject text_o;

                    text_o.lobjecttype = LevelObjectType::TEXT;

                    text_o.lobjectname = objectName;

                    text_o.text = text;

                    text_o.font = font;

                    text_o.fontSize = fontSize;

                    text_o.bold = bold;
                    text_o.italic = italic;
                    text_o.underline = underline;

                    text_o.objectX = textX;
                    text_o.objectY = textY;

                    text_o.objectColor = LevelDesignerUtilities::glColorArrayToSFMLColor(textColor);

                    objectsBuffer.addObject(text_o);

                    LevelDesignerLog::consoleLog("Text '" + std::string(objectName) + "' created.");
                }

                // Separate header from other headers.
                ImGui::NewLine();
            }

            // Sprite.
            if(ImGui::CollapsingHeader("Sprite.")) {
                // Sprite properties.
                static float spriteWidth = 1.0f;
                static float spriteHeight = 1.0f;

                static int spriteX, spriteY;

                static char spritePath[MAX_OBJECTS_TEXT_SIZE / 2];
                
                ImGui::InputFloat("Sprite Width.##sprite", &spriteWidth, 0.1f, 1.0f, "%.1f");
                ImGui::InputFloat("Sprite Height.##sprite", &spriteHeight, 0.1f, 1.0f, "%.1f");

                ImGui::InputInt("Sprite X.##sprite", &spriteX);
                ImGui::InputInt("Sprite Y.##sprite", &spriteY);

                if(ImGui::InputText("Sprite path.##sprite", spritePath, IM_ARRAYSIZE(spritePath))) {
                    if(!texture->loadFromFile(spritePath)) {
                        LevelDesignerLog::consoleLog("Failed to show sprite preview.");
                    } else {
                        LevelDesignerLog::consoleLog("Loaded sprite preview successfully.");
                    }
                }

                if(spriteWidth > 2.0f) {
                    spriteWidth = 2.0f;
                } else if(spriteWidth < 0.0f) {
                    spriteWidth = 0.0f;
                }

                if(spriteHeight > 2.0f) {
                    spriteHeight = 2.0f;
                } else if(spriteHeight < 0.0f) {
                    spriteHeight = 0.0f;
                }

                // Checks.
                if(strlen(spritePath) <= 3) { imgw_log("Sprite path should be specified.", ImVec4 {1.0f, 0.0f, 0.0f, 1.0f}); }

                // Create sprite.
                if(ImGui::Button("Create sprite.")) {
                    LevelObject sprite;

                    sprite.lobjecttype = LevelObjectType::SPRITE;

                    sprite.lobjectname = objectName;

                    sprite.objectSWidth = spriteWidth;
                    sprite.objectSHeight = spriteHeight;

                    sprite.objectX = spriteX;
                    sprite.objectY = spriteY;

                    sprite.spritePath = spritePath;

                    objectsBuffer.addObject(sprite);

                    LevelDesignerLog::consoleLog("Sprite '" + std::string(objectName) + "' created.");
                }

                // Preview.
                ImGui::Image(*texture, sf::Vector2f(DEFAULT_TEXTURE_PREVIEW_WIDTH, DEFAULT_TEXTURE_PREVIEW_HEIGHT), sf::Color::White, sf::Color::White);
            }

            // Separate header from other headers.
            ImGui::NewLine();
        }

        // Objects spawner.
        if(ImGui::CollapsingHeader("Objects Spawner.")) {
            static std::string objectPath;

            for(int objectPosition=0; objectPosition < loadedObjectsBuffer.getBufferSize(); ++objectPosition) {
                // Is it good idea to load texture every frame?
                // if(!ibTexture->loadFromFile(loadedObjectsBuffer.getWithIndex(objectPosition).spritePath.value())) {
                //     LevelDesignerLog::consoleLog("Failed to show IB texture preview.");
                // } else {
                //     LevelDesignerLog::consoleLog("Loaded IB texture preview successfully.");
                // }

                if(ImGui::ImageButton(ibTexturesVector.at(objectPosition), ImVec2 {DEFAULT_TEXTURE_PREVIEW_WIDTH - 10, DEFAULT_TEXTURE_PREVIEW_HEIGHT - 10}, -1, sf::Color::Transparent, sf::Color::White)) {
                    objectsBuffer.addObject(loadedObjectsBuffer.getWithIndex(objectPosition));
                }

                if(ibTexturePos % 2 == 0) {
                    ImGui::SameLine();
                }

                ibTexturePos++;
            }

            ImGui::NewLine();

            ImGui::InputText("Object (.ldobj).", &objectPath);

            if(ImGui::Button("Load.")) {
                int parseCode = ldobjParser.parse((objectPath + std::string(LDOBJ)).c_str());

                if(parseCode == 1) {
                    failedToLoadLDOBJ = true;
                } else if(parseCode == 0) {
                    failedToLoadLDOBJ = false;

                    LevelObject object;

                    object.lobjecttype = LevelObjectType::SPRITE;
    
                    object.lobjectname = ldobjParser.objectName;
    
                    object.spritePath = ldobjParser.spritePath;
    
                    object.objectSWidth = ldobjParser.width;
                    object.objectSHeight = ldobjParser.height;
    
                    object.objectX = ImGui::GetMainViewport()->GetCenter().x;
                    object.objectY = ImGui::GetMainViewport()->GetCenter().y;
    
                    object.objectRotation = ldobjParser.rotation;
    
                    loadedObjectsBuffer.addObject(object);

                    sf::Texture texture;

                    if(!texture.loadFromFile(ldobjParser.spritePath)) {
                        LevelDesignerLog::consoleLog("Failed to allocate texture.");
                    }

                    ibTexturesVector.push_back(texture);

                    LevelDesignerLog::consoleLog("Allocated IB texture preview successfully.");

                    LevelDesignerLog::consoleLog("Loaded LDOBJ successfully.");

                    objectsToLoad.push_back(objectPath);
                }
            }

            if(failedToLoadLDOBJ) {
                imgw_log("Failed to load LDOBJ.", ImVec4 {1.0f, 0.0f, 0.0f, 1.0f});
            }
        }

        // Export/Import.
        if(ImGui::CollapsingHeader("Export/Import.")) {
            static std::string scenePath;

            ImGui::InputText("Scene Path.", &scenePath);

            if(ImGui::Button("Import.")) {
                int parseCode;

                try {
                    parseCode = sceneParser.parse((scenePath + std::string(TDSCENE)).c_str());
                } catch(std::exception const& exception) {
                    corruptedScene = true;

                    parseCode = 1;
                }

                if(parseCode == 1) {
                    failedToLoadScene = true;

                    LevelDesignerLog::consoleLog("Failed to import scene.");
                } else if(parseCode == 0) {
                    failedToLoadScene = false;

                    sFailedToLoadLDOBJ = false;

                    corruptedScene = false;

                    projectName = sceneParser.projectName;

                    backgroundColor[0] = sceneParser.sceneBackground[0];
                    backgroundColor[1] = sceneParser.sceneBackground[1];
                    backgroundColor[2] = sceneParser.sceneBackground[2];

                    for(int posObjectsBuffer=0; posObjectsBuffer < sceneParser.objectsBuffer.getBufferSize(); ++posObjectsBuffer) {
                        objectsBuffer.addObject(sceneParser.objectsBuffer.getWithIndex(posObjectsBuffer));
                    }

                    for(int posLDOBJS=0; posLDOBJS < sceneParser.objectsToLoad.size(); ++posLDOBJS) {
                        // TODO: Write function to load LDOBJ.
                        int ldobjParseCode = ldobjParser.parse((sceneParser.objectsToLoad[posLDOBJS] + std::string(LDOBJ)).c_str());

                        if(ldobjParseCode == 1) {
                            sFailedToLoadLDOBJ = true;
                        } else if(ldobjParseCode == 0) {
                            sFailedToLoadLDOBJ = false;
        
                            LevelObject object;
        
                            object.lobjecttype = LevelObjectType::SPRITE;
            
                            object.lobjectname = ldobjParser.objectName;
            
                            object.spritePath = ldobjParser.spritePath;
            
                            object.objectSWidth = ldobjParser.width;
                            object.objectSHeight = ldobjParser.height;
            
                            object.objectX = ImGui::GetMainViewport()->GetCenter().x;
                            object.objectY = ImGui::GetMainViewport()->GetCenter().y;
            
                            object.objectRotation = ldobjParser.rotation;
            
                            loadedObjectsBuffer.addObject(object);
        
                            sf::Texture texture;
        
                            if(!texture.loadFromFile(ldobjParser.spritePath)) {
                                LevelDesignerLog::consoleLog("(Scene-Import) Failed to allocate texture.");
                            }
        
                            ibTexturesVector.push_back(texture);
        
                            LevelDesignerLog::consoleLog("(Scene-Import) Allocated IB texture preview successfully.");

                            LevelDesignerLog::consoleLog("(Scene-Import) Loaded LDOBJ successfully.");

                            objectsToLoad.push_back(sceneParser.objectsToLoad[posLDOBJS]);

                        }
                    }

                    imported = sceneParser.objectsBuffer.getBufferSize() + sceneParser.objectsToLoad.size();

                    LevelDesignerLog::consoleLog("Scene imported successfully.");
                }

            }

            ImGui::SameLine();

            if(ImGui::Button("Export.")) {
                sceneParser.write((scenePath + std::string(TDSCENE)).c_str(), projectName, backgroundColor, objectsBuffer, objectsToLoad);

                exported = objectsBuffer.getBufferSize() + objectsToLoad.size();

                LevelDesignerLog::consoleLog("Scene exported successfully.");
            }

            if(imported > 0) {
                imgw_log(("Imported " + std::to_string(imported) + " entities.").c_str(), ImVec4 {0.0f, 1.0f, 0.0f, 1.0f});
            }

            if(exported > 0) {
                imgw_log(("Exported " + std::to_string(exported) + " entities.").c_str(), ImVec4 {0.0f, 1.0f, 0.0f, 1.0f});
            }

            if(failedToLoadScene) {
                imgw_log("Failed to load scene.", ImVec4 {1.0f, 0.0f, 0.0f, 1.0f});
            }

            if(sFailedToLoadLDOBJ) {
                imgw_log("Failed to load scene LDOBJS.", ImVec4 {1.0f, 0.0f, 0.0f, 1.0f});
            }

            if(corruptedScene) {
                imgw_log("Failed to load scene. Scene may be corrupted.", ImVec4 {1.0f, 0.0f, 0.0f, 1.0f});
            }
        }

        // TODO: Handle Drag&Drop / click on object.
        for(int bufferPosition=0; bufferPosition < objectsBuffer.getBufferSize(); ++bufferPosition) {
            LevelObject& object = objectsBuffer.getReferenceBuffer()[bufferPosition];

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                ImVec2 mousePosition = ImGui::GetMousePos();

                if(LevelDesignerUtilities::mIntersects(sf::Vector2(object.objectWidth, object.objectHeight), sf::Vector2(object.objectX, object.objectY), mousePosition)) {
                    if(!dragndropping) {
                        if(object.lobjecttype != LevelObjectType::SPRITE && object.lobjecttype != LevelObjectType::CIRCLE) {
                            object.objectX = mousePosition.x - (object.objectWidth / 2);
                            object.objectY = mousePosition.y - (object.objectHeight / 2);
                        } else {
                            object.objectX = mousePosition.x;
                            object.objectY = mousePosition.y;
                        }

                        selectedObjectIndex = bufferPosition;
    
                        selectedObject = objectsBuffer.getWithIndex(bufferPosition);

                        dragndropping = true;
                    }
                } else {
                    dragndropping = false;
                }
            } else {
                dragndropping = false;
            }

            if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                ImVec2 mousePosition = ImGui::GetMousePos();

                if(LevelDesignerUtilities::mIntersects(sf::Vector2(object.objectWidth, object.objectHeight), sf::Vector2(object.objectX, object.objectY), mousePosition)) {
                    selectedObjectIndex = bufferPosition;

                    selectedObject = objectsBuffer.getWithIndex(bufferPosition);
                }
            }
        }

        // Draw object editor UI.
        ObjectEditorUI::show(objectsBuffer.getReferenceBuffer(), selectedObjectIndex);

        // End ImGui window.
        ImGui::End();

        // Set LevelDesignerSettings -> backgroundColor.
        ldsettings.backgroundColor = sf::Color {
            static_cast<sf::Uint8>(LevelDesignerUtilities::glColorToSFMLColor(backgroundColor[0])),
            static_cast<sf::Uint8>(LevelDesignerUtilities::glColorToSFMLColor(backgroundColor[1])),
            static_cast<sf::Uint8>(LevelDesignerUtilities::glColorToSFMLColor(backgroundColor[2])),

            255
        };

        // Clear screen.
        window.clear(ldsettings.backgroundColor);

        // Draw objects from buffer.
        for(int bufferPosition=0; bufferPosition < objectsBuffer.getBufferSize(); ++bufferPosition) {
            ObjectDrawer::drawObject(objectsBuffer.getReferenceBuffer()[bufferPosition], window);
        }

        // Render ImGui window.
        ImGui::SFML::Render(window);

        // Update screen.
        window.display();
    }

    // Shutdown ImGui.
    ImGui::SFML::Shutdown();

    // Return SUCCESS.
    return 0;
}
