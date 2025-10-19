#include <SFML/Graphics.hpp>
#include "Personaggio.h"
#include "Mappa.h"
#include <cmath>

using namespace sf;

template<typename T>
T clamp(T v, T lo, T hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

// Funzione per verificare collisioni tra rettangoli
bool checkCollision(const FloatRect& rect1, const FloatRect& rect2) {
    return rect1.intersects(rect2);
}

int main() {
    RenderWindow window(VideoMode(1400, 900), "SFML con CMake");
    Clock clock;

    Mappa mappa(Vector2f(1400.f, 900.f)); // stessa dimensione della finestra
    Personaggio player(Vector2f(50.f, 50.f), Vector2f(30.f, 30.f));

    while (window.isOpen()) {
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        // Movimento con WASD
        Vector2f direzione(0.f, 0.f);
        if (Keyboard::isKeyPressed(Keyboard::W)) direzione.y -= 1.f;
        if (Keyboard::isKeyPressed(Keyboard::S)) direzione.y += 1.f;
        if (Keyboard::isKeyPressed(Keyboard::A)) direzione.x -= 1.f;
        if (Keyboard::isKeyPressed(Keyboard::D)) direzione.x += 1.f;

        // Normalizza la direzione
        if (direzione.x != 0.f || direzione.y != 0.f) {
            float len = std::sqrt(direzione.x*direzione.x + direzione.y*direzione.y);
            direzione /= len;
        }

        // Salva la posizione prima del movimento
        Vector2f oldPos = player.getPosizione();

        // Prova a muovere il player
        player.muovi(direzione, deltaTime);

        // Ottieni la nuova posizione e bounds
        Vector2f newPos = player.getPosizione();
        FloatRect playerBounds = player.getGlobalBounds();

        // Controlla collisioni con i muri
        bool collisione = false;
        for (const auto& muro : mappa.getMuri()) {
            if (checkCollision(playerBounds, muro.getGlobalBounds())) {
                collisione = true;
                break;
            }
        }

        // 🔹 Controlla collisioni con gli ostacoli
        if (!collisione) {
            for (const auto& ostacolo : mappa.getOstacoli()) {
                if (checkCollision(playerBounds, ostacolo.getGlobalBounds())) {
                    collisione = true;
                    break;
                }
            }
        }

        // Se c'è collisione, ripristina la posizione precedente
        if (collisione) {
            player.setPosizione(oldPos);
        } else {
            // Limita il personaggio all'interno della mappa (solo se non collide)
            FloatRect bounds = mappa.getBounds();
            Vector2f pos = player.getPosizione();
            Vector2f size = player.getDimensione();
            Vector2f half = size / 2.f;

            pos.x = clamp(pos.x, bounds.left + half.x, bounds.left + bounds.width - half.x);
            pos.y = clamp(pos.y, bounds.top + half.y, bounds.top + bounds.height - half.y);

            player.setPosizione(pos);
        }

        window.clear();
        mappa.disegna(window);
        player.disegna(window);
        window.display();
    }

    return 0;
}