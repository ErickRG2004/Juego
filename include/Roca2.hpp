#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#pragma once

class Roca2
{
public:
    Roca2(sf::Vector2f position, const std::string &imagePath)
    {
        shape.setSize(sf::Vector2f(70, 70));
        shape.setPosition(position);
        shape.setFillColor(sf::Color::Black);

        if (!imageTexture.loadFromFile(imagePath))
        {
            std::cerr << "Error: No se pudo cargar la imagen del personaje.\n";
        }
        imageSprite.setTexture(imageTexture);
        imageSprite.setScale(0.5f, 0.5f);
        updateImagePosition();
    }

    void draw(sf::RenderWindow &window) const
    {
        window.draw(shape);
        window.draw(imageSprite);
    }

    sf::FloatRect getBounds() const
    {
        return shape.getGlobalBounds();
    }

private:
    sf::RectangleShape shape;
    sf::Texture imageTexture;
    sf::Sprite imageSprite;

    void updateImagePosition()
    {
        float scaleFactorX = shape.getSize().x / 130;
        float scaleFactorY = shape.getSize().y / 130;
        float scaleFactor = std::min(scaleFactorX, scaleFactorY);
        imageSprite.setScale(scaleFactor, scaleFactor);

        sf::FloatRect spriteBounds = imageSprite.getGlobalBounds();
        sf::Vector2f position = shape.getPosition();
        imageSprite.setPosition(
            position.x + (shape.getSize().x - spriteBounds.width) / 2,
            position.y + (shape.getSize().y - spriteBounds.height) / 1.7);
    }
};