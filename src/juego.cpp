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

    void draw(sf::RenderWindow& window) const {
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
    Personaje(sf::Vector2f position, sf::Color color, const std::string& imagePath) : vidas(3) {
        shape.setSize(sf::Vector2f(30, 50));
        shape.setPosition(position);
        shape.setFillColor(color);

        if (!imageTexture.loadFromFile(imagePath)) {
            std::cerr << "Error: No se pudo cargar la imagen del personaje.\n";
        }
        imageSprite.setTexture(imageTexture);
        imageSprite.setScale(0.5f, 0.5f);
        updateImagePosition();
    }

    void move(float offsetX, float offsetY) {
        shape.move(offsetX, offsetY);
        updateImagePosition();
    }

    void shoot(std::vector<Projectile>& projectiles, sf::Vector2f direction, float speed, sf::Clock& clock, float delay) {
        if (clock.getElapsedTime().asSeconds() >= delay) {
            sf::Vector2f position = shape.getPosition() + sf::Vector2f(shape.getSize().x, shape.getSize().y);
            projectiles.emplace_back(position, direction, speed);
            clock.restart();
        }
    }

    void perderVida() {
        if (vidas > 0) {
            vidas--;
        }
    }

    int getVidas() const {
        return vidas;
    }

    bool estaVivo() const {
        return vidas > 0;
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
    int vidas;

    void updateImagePosition() {
        float scaleFactorX = shape.getSize().x / 200;
        float scaleFactorY = shape.getSize().y / 200;
        float scaleFactor = std::min(scaleFactorX, scaleFactorY);
        imageSprite.setScale(scaleFactor, scaleFactor);

        sf::FloatRect spriteBounds = imageSprite.getGlobalBounds();
        sf::Vector2f position = shape.getPosition();
        imageSprite.setPosition(
            position.x + (shape.getSize().x - spriteBounds.width) / 2,
            position.y + (shape.getSize().y - spriteBounds.height) / 1.3
        );
    }
};

double velocidad = 0.2;

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
    Personaje character(sf::Vector2f(1000, 400), sf::Color::Red, "C:/Users/1105334954/Downloads/jug1.png");
    Personaje character2(sf::Vector2f(200, 400), sf::Color::Blue, "C:/Users/1105334954/Downloads/jug2.png");

    // Lista de proyectiles
    std::vector<Projectile> projectiles;

    float fireDelay = 0.5f;
    sf::Clock clock1;
    sf::Clock clock2;

    // Cargar fuente para mostrar vidas
    /*sf::Font font;
    if (!font.loadFromFile("./assets/Minecraft.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente.\n";
        return -1;
    }*/

    // Crear textos para mostrar vidas
    /*sf::Text vidasP1("Vidas: 3", font, 20);
    sf::Text vidasP2("Vidas: 3", font, 20);*/

    /*vidasP1.setPosition(10, 10);
    vidasP2.setPosition(1100, 10);*/

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Movimiento del personaje 1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            character.move(-velocidad, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            character.move(velocidad, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            character.move(0, -velocidad);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            character.move(0, velocidad);
        }

        // Movimiento del personaje 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            character2.move(-velocidad, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            character2.move(velocidad, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            character2.move(0, -velocidad);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            character2.move(0, velocidad);
        }

        // Disparo del personaje 1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            character.shoot(projectiles, sf::Vector2f(-1, 0), 5.f, clock1, fireDelay);
        }

        // Disparo del personaje 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            character2.shoot(projectiles, sf::Vector2f(1, 0), 5.f, clock2, fireDelay);
        }

        // Actualizar proyectiles y detectar colisiones
        for (auto it = projectiles.begin(); it != projectiles.end();) {
            it->update();
            bool impactado = false;

            if (character.getBounds().intersects(it->getBounds())) {
                character.perderVida();
                impactado = true;
            }
            if (character2.getBounds().intersects(it->getBounds())) {
                character2.perderVida();
                impactado = true;
            }

            if (impactado) {
                it = projectiles.erase(it);
            } else {
                ++it;
            }
        }

        // Verificar si algún personaje perdió todas las vidas
        if (!character.estaVivo()) {
            std::cout << "¡El personaje 1 ha perdido todas sus vidas! El jugador 2 gana.\n";
            window.close();
        }
        if (!character2.estaVivo()) {
            std::cout << "¡El personaje 2 ha perdido todas sus vidas! El jugador 1 gana.\n";
            window.close();
        }

        // Actualizar textos de vidas
        /*vidasP1.setString("Vidas: " + std::to_string(character.getVidas()));
        vidasP2.setString("Vidas: " + std::to_string(character2.getVidas()));*/

        // Dibujar todo
        window.clear();
        window.draw(backgroundSprite);
        character.draw(window);
        character2.draw(window);
        for (const auto& projectile : projectiles) {
            projectile.draw(window);
        }
        /*window.draw(vidasP1);
        window.draw(vidasP2);*/
        window.display();
    }

    return 0;
}
