#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <Projectile.hpp>
#pragma once

class Personaje
{
public:
    Personaje(sf::Vector2f position, sf::Color color, const std::string &imagePath) : vidas(3)
    {
        shape.setSize(sf::Vector2f(30, 50));
        shape.setPosition(position);
        shape.setFillColor(color);

        if (!imageTexture.loadFromFile(imagePath))
        {
            std::cerr << "Error: No se pudo cargar la imagen del personaje.\n";
        }
        imageSprite.setTexture(imageTexture);
        imageSprite.setScale(0.5f, 0.5f);
        updateImagePosition();
    }

    void move(float offsetX, float offsetY)
    {
        shape.move(offsetX, offsetY);
        updateImagePosition();
    }

    void shoot(std::vector<Projectile> &projectiles, sf::Vector2f direction, float speed, sf::Clock &clock, float delay)
    {
        if (clock.getElapsedTime().asSeconds() >= delay)
        {
            sf::Vector2f position = shape.getPosition() + sf::Vector2f(shape.getSize().x, shape.getSize().y);
            projectiles.emplace_back(position, direction, speed);
            clock.restart();
        }
    }

    void perderVida()
    {
        if (vidas > 0)
        {
            vidas--;
        }
    }

    int getVidas() const
    {
        return vidas;
    }

    bool estaVivo() const
    {
        return vidas > 0;
    }

    void draw(sf::RenderWindow &window)
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
    int vidas;

    void updateImagePosition()
    {
        float scaleFactorX = shape.getSize().x / 100;
        float scaleFactorY = shape.getSize().y / 100;
        float scaleFactor = std::min(scaleFactorX, scaleFactorY);
        imageSprite.setScale(scaleFactor, scaleFactor);

        sf::FloatRect spriteBounds = imageSprite.getGlobalBounds();
        sf::Vector2f position = shape.getPosition();
        imageSprite.setPosition(
            position.x + (shape.getSize().x - spriteBounds.width) / 2,
            position.y + (shape.getSize().y - spriteBounds.height) / 1.3);
    }
};