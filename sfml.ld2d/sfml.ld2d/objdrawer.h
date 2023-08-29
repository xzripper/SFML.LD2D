#ifndef SFML_LD2D_OBJDRAWER
#define SFML_LD2D_OBJDRAWER

#include "../SFML\include\SFML\Graphics.hpp"

#include "consts.h"

#include "lobject.h"
#include "lobjectype.h"

#include "shadertypes.h"

#include "ldlog.h"

#include <iostream>

class ObjectDrawer {
public:
static void drawObject(LevelObject& object, sf::RenderWindow &window) {
    if(!object.hidden) {
        if(object.lobjecttype == LevelObjectType::RECTANGLE) {
            sf::RectangleShape rectangleShape(sf::Vector2f(object.objectWidth, object.objectHeight));

            rectangleShape.setPosition(sf::Vector2f(object.objectX, object.objectY));

            if(object.objectRotation.has_value()) {
                rectangleShape.setOrigin(object.objectWidth / 2.f, object.objectHeight / 2.f);

                rectangleShape.setRotation(object.objectRotation.value());
            }

            rectangleShape.setFillColor(object.objectColor);

            window.draw(rectangleShape);
        } else if(object.lobjecttype == LevelObjectType::CIRCLE) {
                if(!object.objectRadius.has_value()) {
                    LevelDesignerLog::windowLog("objdrawer.h: missing 'objectRadius'.", window);

                    return;
                } else {
                    if(object.objectRadius.value() <= 0) {
                        LevelDesignerLog::windowLog("objdrawer.h: 'objectRadius' should be more than 0.", window);

                        return;
                    }

                    sf::CircleShape circleShape(object.objectRadius.value());

                    circleShape.setScale(object.objectWidth, object.objectHeight);

                    circleShape.setPosition(sf::Vector2f(object.objectX, object.objectY));

                    // if(!object.sizeNormalized) {
                    //     object.objectWidth = object.objectWidth + object.objectRadius.value();
                    //     object.objectHeight = object.objectHeight + object.objectRadius.value();

                    //     object.sizeNormalized = true;
                    // }

                    if(object.objectRotation.has_value()) {
                        circleShape.setOrigin(object.objectWidth / 2.f, object.objectHeight / 2.f);

                        circleShape.setRotation(object.objectRotation.value());
                    }

                    circleShape.setFillColor(object.objectColor);

                    window.draw(circleShape);
                }
            } else if(object.lobjecttype == LevelObjectType::TEXT) {
                if(!object.text.has_value() || !object.font.has_value() || !object.fontSize.has_value()) {
                    LevelDesignerLog::windowLog("objdrawer.h: missing 'text' or 'font' or 'fontSize'.", window);

                    return;
                }

                sf::Font font;

                std::string fontPath;

                std::string _fontPath = object.font.value();

                if(object.font.value().find(BUILTIN) == 0) {
                    fontPath = _fontPath.replace(0, sizeof(BUILTIN) - 1, BUILTIN_PATH) + ".ttf";
                } else {
                    fontPath = _fontPath;
                }

                if(!font.loadFromFile(fontPath)) {
                    LevelDesignerLog::windowLog("objdrawer.h: failed to load font: '" + fontPath + "'.", window);

                    font.loadFromFile(DEFAULT_WINDOW_FONT);
                }

                font.setSmooth(true);

                sf::Text text(object.text.value(), font, object.fontSize.value());

                text.setPosition(object.objectX, object.objectY);

                if(!object.sizeNormalized) {
                    object.objectWidth = object.text.value().size() * object.fontSize.value();
                    object.objectHeight = object.fontSize.value();

                    object.sizeNormalized = true;
                }

                if(object.objectRotation.has_value()) {
                    text.setRotation(object.objectRotation.value());
                }

                text.setColor(object.objectColor);

                text.setStyle((object.bold.value() ? sf::Text::Bold : 0) | (object.italic.value() ? sf::Text::Italic : 0) | (object.underline.value() ? sf::Text::Underlined : 0));

                window.draw(text);
            } else if(object.lobjecttype == LevelObjectType::SPRITE) {
                if(!object.spritePath.has_value() || !object.objectSWidth.has_value() || !object.objectSHeight.has_value()) {
                    LevelDesignerLog::windowLog("objdrawer.h: missing 'spritePath' or 'objectSWidth. or 'objectSHeight'.", window);

                    return;
                }

                sf::Texture objectTexture;

                if(!objectTexture.loadFromFile(object.spritePath.value())) {
                    LevelDesignerLog::windowLog("objdrawer.h: failed to load sprite: '" + object.spritePath.value() + "'.", window);
                }

                objectTexture.setSmooth(true);

                sf::Sprite objectSprite(objectTexture);

                objectSprite.setScale(object.objectSWidth.value(), object.objectSHeight.value());

                objectSprite.setPosition(object.objectX, object.objectY);

                if(!object.sizeNormalized) {
                    object.objectWidth = objectTexture.getSize().x;
                    object.objectHeight = objectTexture.getSize().y;

                    object.sizeNormalized = true;

                }

                if(object.objectRotation.has_value()) {
                    objectSprite.setOrigin(object.objectWidth / 2.f, object.objectHeight / 2.f);

                    objectSprite.setRotation(object.objectRotation.value());
                }

                if(object.shader.value() == ShaderTypes::NO_SHADERS) {
                    window.draw(objectSprite);
                } else {
                    if(object.shader.value() == ShaderTypes::PIXELATE) {
                        sf::Shader pixelateShader;

                        if(!pixelateShader.loadFromFile(SHADER_PIXELATE_PATH, sf::Shader::Fragment)) {
                            LevelDesignerLog::windowLog("Failed to load 'pixelate' shader.", window);
                        }

                        pixelateShader.setParameter("texture", sf::Shader::CurrentTexture);

                        pixelateShader.setParameter("pixelThreshold", object.shader_pixelThreshold.value());

                        window.draw(objectSprite, &pixelateShader);
                    } else if(object.shader.value() == ShaderTypes::BLUR) {
                        sf::Shader blurShader;

                        if(!blurShader.loadFromFile(SHADER_BLUR_PATH, sf::Shader::Fragment)) {
                            LevelDesignerLog::windowLog("Failed to load 'blur' shader.", window);
                        }

                        blurShader.setParameter("texture", sf::Shader::CurrentTexture);

                        blurShader.setParameter("blurRadius", object.shader_blurRadius.value());

                        window.draw(objectSprite, &blurShader);
                    }
                }
            } else {
                return;
            }
        }
    }
};

#endif
