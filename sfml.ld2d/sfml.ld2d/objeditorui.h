#ifndef SFML_LD2D_OBJEDITORUI
#define SFML_LD2D_OBJEDITORUI

#include "../imgui\include\imgui.h"
#include "../imgui\include\imgui_stdlib.h"

#include "consts.h"

#include "objbuff.h"

#include "lobject.h"

#include "lobjectype.h"

#include <vector>

class ObjectEditorUI {
public:
    static inline int dynHeight = OBJECT_EDITOR_UI_HEIGHT;
    static inline int dynX = OBJECT_EDITOR_UI_X;

    static void show(std::vector<LevelObject> &buffer, int selectedObjectIndex) {
        bool open = true;

        ImGui::Begin(OBJECT_EDITOR_UI_TITLE, &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGui::SetWindowSize(ImVec2 {OBJECT_EDITOR_UI_WIDTH, ObjectEditorUI::dynHeight});

        ImGui::SetWindowPos(ImVec2 {ObjectEditorUI::dynX, OBJECT_EDITOR_UI_Y});

        if(selectedObjectIndex != -1) {
            LevelObject& object = buffer[selectedObjectIndex];

            if(object.lobjecttype == LevelObjectType::RECTANGLE) {
                ImGui::Text("Editing...: LevelObjectType::RECTANGLE.");

                ImGui::NewLine();

                int objectRotation = object.objectRotation.value_or(0);

                ImGui::Text("General.");

                ImGui::InputText("Object Name.##rect", &object.lobjectname);

                ImGui::NewLine();

                ImGui::Text("Transform.");

                ImGui::InputInt("Object Width.##rect", &object.objectWidth);
                ImGui::InputInt("Object Height.##rect", &object.objectHeight);

                ImGui::InputInt("Object X.##rect", &object.objectX);
                ImGui::InputInt("Object Y.##rect", &object.objectY);

                if(ImGui::InputInt("Object Rotation.##rect", &objectRotation)) { object.objectRotation = objectRotation; }

                ImGui::NewLine();

                ImGui::Text("Other.");

                ImGui::Checkbox("Hidden?##rect", &object.hidden);
            } else if(object.lobjecttype == LevelObjectType::CIRCLE) {
                ImGui::Text("Editing...: LevelObjectType::CIRCLE.");

                ImGui::NewLine();

                int objectRadius = object.objectRadius.value_or(0);

                int objectRotation = object.objectRotation.value_or(0);

                ImGui::Text("General.");

                ImGui::InputText("Object Name.##circle", &object.lobjectname);

                if(ImGui::InputInt("Object Radius.##circle", &objectRadius)) { object.objectRadius = objectRadius; }

                ImGui::NewLine();

                ImGui::Text("Transform.");

                ImGui::InputInt("Object Width.##circle", &object.objectWidth);
                ImGui::InputInt("Object Height.##circle", &object.objectHeight);

                ImGui::InputInt("Object X.##circle", &object.objectX);
                ImGui::InputInt("Object Y.##circle", &object.objectY);

                if(ImGui::InputInt("Object Rotation.##rect", &objectRotation)) { object.objectRotation = objectRotation; }

                ImGui::NewLine();

                ImGui::Text("Other.");

                ImGui::Checkbox("Hidden?##circle", &object.hidden);
            } else if(object.lobjecttype == LevelObjectType::TEXT) {
                ImGui::Text("Editing...: LevelObjectType::TEXT.");

                ImGui::NewLine();

                std::string text = object.text.value();
                std::string font = object.font.value();

                int fontSize = object.fontSize.value();

                int objectRotation = object.objectRotation.value_or(0);

                bool bold = object.bold.value();
                bool italic = object.italic.value();
                bool underline = object.underline.value();

                ImGui::Text("General.");

                ImGui::InputText("Object Name.##text", &object.lobjectname);

                if(ImGui::InputText("Text.##text", &text)) { object.text = text; }

                if(ImGui::InputText("Font.##text", &font)) { object.font = font; }

                if(ImGui::InputInt("Font size.##text", &fontSize)) { object.fontSize = fontSize; }

                ImGui::NewLine();

                ImGui::Text("Transform.");

                ImGui::InputInt("Object X.##text", &object.objectX);
                ImGui::InputInt("Object Y.##text", &object.objectY);

                if(ImGui::InputInt("Object Rotation.##text", &objectRotation)) { object.objectRotation = objectRotation; }

                ImGui::NewLine();

                ImGui::Text("Other.");

                if(ImGui::Checkbox("Bold.", &bold)) {
                    object.bold = bold;
                } ImGui::SameLine();

                if(ImGui::Checkbox("Italic.", &italic)) {
                    object.italic = italic;
                } ImGui::SameLine();

                if(ImGui::Checkbox("Underline.", &underline)) {
                    object.underline = underline;
                } ImGui::SameLine();

                ImGui::Checkbox("Hidden?##text", &object.hidden);
            } else if(object.lobjecttype == LevelObjectType::SPRITE) {
                ImGui::Text("Editing...: LevelObjectType::SPRITE.");

                ImGui::NewLine();

                std::string spritePath = object.spritePath.value();

                int objectRotation = object.objectRotation.value_or(0);

                float sWidth = object.objectSWidth.value_or(1.0f);
                float sHeight = object.objectSHeight.value_or(1.0f);

                int currentShader = object.shader.value();

                float _shader_pixelThreshold = object.shader_pixelThreshold.value();
                float _shader_blurRadius = object.shader_blurRadius.value();

                ImGui::Text("General.");

                ImGui::InputText("Object Name.##sprite", &object.lobjectname);

                if(ImGui::InputText("Sprite Path.##sprite", &spritePath)) { object.spritePath = spritePath; }

                ImGui::NewLine();

                ImGui::Text("Transform.");

                if(ImGui::InputFloat("Object Width.##sprite", &sWidth, 0.1f, 1.0f, "%.1f")) {
                    if(sWidth > 2.0f) {
                        sWidth = 2.0f;
                    } else if(sWidth < 0.0f) {
                        sWidth = 0.0f;
                    }

                    object.objectSWidth = sWidth;
                }

                if(ImGui::InputFloat("Object Height.##sprite", &sHeight, 0.1f, 1.0f, "%.1f")) {
                    if(sHeight > 2.0f) {
                        sHeight = 2.0f;
                    } else if(sHeight < 0.0f) {
                        sHeight = 0.0f;
                    }

                    object.objectSHeight = sHeight;
                }

                ImGui::InputInt("Object X.##sprite", &object.objectX);
                ImGui::InputInt("Object Y.##sprite", &object.objectY);

                if(ImGui::InputInt("Object Rotation.##sprite", &objectRotation)) { object.objectRotation = objectRotation; }

                ImGui::NewLine();

                ImGui::Text("Shaders.");

                if(ImGui::Combo("Shader.", &currentShader, "None.\0Pixelate.\0Blur.\0\0")) {
                    object.shader = currentShader;
                }

                ImGui::NewLine();

                ImGui::Text("Shader Parameters.");

                if(currentShader == 1) {
                    ImGui::Text("Pixel threshold (shader_pixelThreshold).");

                    if(ImGui::InputFloat("##sprite_shader_pixel_threshold_parameter", &_shader_pixelThreshold, 0.01, 0.1, "%.2f")) {
                        if(_shader_pixelThreshold > 1.0f) {
                            _shader_pixelThreshold = 1.0f;
                        } else if(_shader_pixelThreshold < 0.0f) {
                            _shader_pixelThreshold = 0.0f;
                        }

                        object.shader_pixelThreshold = _shader_pixelThreshold;
                    }
                } else if(currentShader == 2) {
                    ImGui::Text("Blur radius (shader_blurRadius).");

                    if(ImGui::InputFloat("##sprite_shader_blur_radius_parameter", &_shader_blurRadius, 0.01, 0.1, "%.2f")) {
                        if(_shader_blurRadius > 1.0f) {
                            _shader_blurRadius = 1.0f;
                        } else if(_shader_blurRadius < 0.0f) {
                            _shader_blurRadius = 0.0f;
                        }

                        object.shader_blurRadius = _shader_blurRadius;
                    }
                } else {
                    ImGui::Text("No shaders applied.");
                }

                ImGui::NewLine();

                ImGui::Text("Other.");

                ImGui::Checkbox("Hidden?##sprite", &object.hidden);
            }
        }

        ImGui::End();
    }
};

#endif
