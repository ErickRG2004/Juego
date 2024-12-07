#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Projectile {
public:
    Projectile(sf::Vector2f position, sf::Vector2f direction, float speed) {
        shape.setRadius(5.f);
        shape.setFillColor(sf::Color::Yellow);
        shape.setPosition(position);
        velocity = direction * speed;
    }

    void update() {
        shape.move(velocity);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

class Personaje {
public:
    Personaje(sf::Vector2f position, sf::Color color, const std::string& imagePath) {
        // Inicializar el cuerpo
        shape.setSize(sf::Vector2f(50, 50));
        shape.setPosition(position);
        shape.setFillColor(color);

        // Cargar la textura de la imagen
        if (!imageTexture.loadFromFile(imagePath)) {
            std::cerr << "Error: No se pudo cargar la imagen del personaje.\n";
        }
        imageSprite.setTexture(imageTexture);
        imageSprite.setScale(0.5f, 0.5f); // Escalar imagen si es necesario
        updateImagePosition();
    }

    void move(float offsetX, float offsetY) {
        shape.move(offsetX, offsetY);
        updateImagePosition();
    }

    void shoot(std::vector<Projectile>& projectiles, sf::Vector2f direction, float speed, sf::Clock& clock, float delay) {
        if (clock.getElapsedTime().asSeconds() >= delay) {
            sf::Vector2f position = shape.getPosition() + sf::Vector2f(shape.getSize().x / 2, shape.getSize().y / 2);
            projectiles.emplace_back(position, direction, speed);
            clock.restart();
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(imageSprite);
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }

private:
    sf::RectangleShape shape;
    sf::Texture imageTexture;
    sf::Sprite imageSprite;

    void updateImagePosition() {
        // Ajustar la posición del sprite justo encima del jugador
        sf::Vector2f position = shape.getPosition();
        imageSprite.setPosition(position.x, position.y - 50); // 50 píxeles arriba
    }
};

double velocidad = 0.1;

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "DinoChrome");

    // Cargar la textura del fondo
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:/Users/1105334954/Downloads/campo.jpg")) {
        std::cerr << "Error: No se pudo cargar la imagen de fondo.\n";
        return -1;
    }

    // Crear el sprite del fondo
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Ajustar el tamaño del fondo al de la ventana
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    backgroundSprite.setScale(scaleX, scaleY);

    // Crear los personajes con sus imágenes
    Personaje character(sf::Vector2f(400, 300), sf::Color::Red, "C:/Users/1105334954/Downloads/J2.png");
    Personaje character2(sf::Vector2f(200, 300), sf::Color::Blue, "C:/Users/1105334954/Downloads/J1.png");

    // Lista de proyectiles
    std::vector<Projectile> projectiles;

    float speedCharacter1 = 0.5f;
    float speedCharacter2 = 0.5f;

    float fireDelay = 0.5f;
    sf::Clock clock1;
    sf::Clock clock2;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Movimiento del personaje 1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            character.move(velocidad * -1, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            character.move(velocidad, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            character.move(0, velocidad * -1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            character.move(0, velocidad);
        }

        // Movimiento del personaje 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            character2.move(velocidad * -1, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            character2.move(velocidad, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            character2.move(0, velocidad * -1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            character2.move(0, velocidad);
        }

        // Disparo del personaje 1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            character.shoot(projectiles, sf::Vector2f(-1, 0), speedCharacter1, clock1, fireDelay);
        }

        // Disparo del personaje 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            character2.shoot(projectiles, sf::Vector2f(1, 0), speedCharacter2, clock2, fireDelay);
        }

        // Actualizar proyectiles
        for (auto& projectile : projectiles) {
            projectile.update();
        }

        // Dibujar todo
        window.clear();
        window.draw(backgroundSprite); // Dibujar la imagen de fondo
        character.draw(window);
        character2.draw(window);
        for (auto& projectile : projectiles) {
            projectile.draw(window);
        }
        window.display();
    }

    return 0;
}
