#include <SFML/Graphics.hpp>
#include "Personaggio.h"

using namespace sf;

int main() {
    RenderWindow window(VideoMode(1200, 1000), "SFML con CMake");
    Clock clock;

    Personaggio player(Vector2f(50.f, 50.f), Vector2f(30.f, 30.f));

    while (window.isOpen()) {
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }
        float deltaTime = clock.restart().asSeconds();

        // Movimento base con WASD
        Vector2f direzione(0.f, 0.f);
        if (Keyboard::isKeyPressed(Keyboard::W)) direzione.y -= 1.f;
        if (Keyboard::isKeyPressed(Keyboard::S)) direzione.y += 1.f;
        if (Keyboard::isKeyPressed(Keyboard::A)) direzione.x -= 1.f;
        if (Keyboard::isKeyPressed(Keyboard::D)) direzione.x += 1.f;

        player.muovi(direzione, deltaTime);

        window.clear();
        player.disegna(window);
        window.display();
    }

    return 0;
}