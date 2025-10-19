#include <SFML/Graphics.hpp>
#include "Personaggio.h"
#include "Mappa.h"
#include <cmath>

using namespace sf;

template<typename T>
T clamp(T v, T lo, T hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

bool checkCollision(const FloatRect& rect1, const FloatRect& rect2) {
    return rect1.intersects(rect2);
}

int main() {
    RenderWindow window(VideoMode(1400, 900), "SFML con CMake");
    Clock clock;

    Mappa mappa(Vector2f(1400.f, 900.f));
    Personaggio player(Vector2f(50.f, 50.f), Vector2f(30.f, 30.f));

    FloatRect mappaBounds(0.f, 0.f, 1400.f, 900.f);

    float apertura = 200.f;
    float centro = mappaBounds.height / 2.f;
    float yTop = centro - apertura / 2.f;
    float yBottom = centro + apertura / 2.f;

    while (window.isOpen()) {
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        Vector2f direzione(0.f, 0.f);
        if (Keyboard::isKeyPressed(Keyboard::W)) direzione.y -= 1.f;
        if (Keyboard::isKeyPressed(Keyboard::S)) direzione.y += 1.f;
        if (Keyboard::isKeyPressed(Keyboard::A)) direzione.x -= 1.f;
        if (Keyboard::isKeyPressed(Keyboard::D)) direzione.x += 1.f;

        if (direzione.x != 0.f || direzione.y != 0.f) {
            float len = std::sqrt(direzione.x*direzione.x + direzione.y*direzione.y);
            direzione /= len;
        }

        Vector2f oldPos = player.getPosizione();

        player.muovi(direzione, deltaTime);

        Vector2f newPos = player.getPosizione();
        Vector2f size = player.getDimensione();
        FloatRect playerBounds(newPos.x, newPos.y, size.x, size.y);

        if (newPos.x < 0) {
            float playerCenterY = newPos.y + size.y / 2.f;
            if (playerCenterY > yTop && playerCenterY < yBottom) {
                window.close();
                return 0;
            }
        }

        bool collisione = false;
        for (const auto& muro : mappa.getMuri()) {
            if (checkCollision(playerBounds, muro.getGlobalBounds())) {
                collisione = true;
                break;
            }
        }

        if (!collisione) {
            for (const auto& ostacolo : mappa.getOstacoli()) {
                if (checkCollision(playerBounds, ostacolo.getGlobalBounds())) {
                    collisione = true;
                    break;
                }
            }
        }

        if (collisione) {
            player.setPosizione(oldPos);
        } else {
            Vector2f pos = player.getPosizione();
            Vector2f half = size / 2.f;

            pos.x = clamp(pos.x, -size.x, mappaBounds.left + mappaBounds.width - half.x);
            pos.y = clamp(pos.y, mappaBounds.top + half.y, mappaBounds.top + mappaBounds.height - half.y);

            player.setPosizione(pos);
        }

        window.clear();
        mappa.disegna(window);
        player.disegna(window);
        window.display();
    }

    return 0;
}