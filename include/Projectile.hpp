#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#pragma once

class Projectile
{
public:
    Projectile(sf::Vector2f position, sf::Vector2f direction, float speed)
    {
        shape.setRadius(5.f);
        shape.setFillColor(sf::Color::Black);
        shape.setPosition(position);
        velocity = direction * speed;
    }

    void update()
    {
        shape.move(velocity);
    }

    void draw(sf::RenderWindow &window) const
    {
        window.draw(shape);
    }

    sf::FloatRect getBounds() const
    {
        return shape.getGlobalBounds();
    }

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
};
