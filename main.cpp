#include <SFML/Graphics.hpp>
#include "Personaggio.h"
#include "Mappa.h"
#include "Nemico.h"
#include "NavigationGrind.h"
#include <vector>
#include <cmath>
#include <random>

using namespace sf;

template<typename T>
T clamp(T v, T lo, T hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

bool checkCollision(const FloatRect& rect1, const FloatRect& rect2) {
    return rect1.intersects(rect2);
}

// Funzione per verificare se una posizione è valida per lo spawn
bool isPosizioneValida(const Vector2f& pos, const Vector2f& size, const Mappa& mappa) {
    FloatRect bounds(pos.x, pos.y, size.x, size.y);

    // Controlla collisioni con muri
    for (const auto& muro : mappa.getMuri()) {
        if (bounds.intersects(muro.getGlobalBounds())) {
            return false;
        }
    }

    // Controlla collisioni con ostacoli
    for (const auto& ostacolo : mappa.getOstacoli()) {
        if (bounds.intersects(ostacolo.getGlobalBounds())) {
            return false;
        }
    }

    return true;
}

// Funzione per trovare una posizione di spawn sicura per il player
Vector2f trovaSpawnSicuroPlayer(const Mappa& mappa, const Vector2f& playerSize) {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Area di spawn preferita a destra (vicino al muro destro ma non dentro)
    std::uniform_real_distribution<float> distX(1200.f, 1350.f); // Più lontano dal bordo destro
    std::uniform_real_distribution<float> distY(100.f, 800.f);   // Lontano dai bordi superiore/inferiore

    int tentativi = 0;
    while (tentativi < 100) {
        tentativi++;
        Vector2f pos(distX(gen), distY(gen));

        if (isPosizioneValida(pos, playerSize, mappa)) {
            return pos;
        }
    }

    // Fallback: cerca sistematicamente una posizione valida
    for (float x = 1200.f; x >= 100.f; x -= 50.f) {
        for (float y = 100.f; y < 800.f; y += 50.f) {
            Vector2f pos(x, y);
            if (isPosizioneValida(pos, playerSize, mappa)) {
                return pos;
            }
        }
    }

    // Ultimo fallback: posizione fissa che dovrebbe essere sicura
    return Vector2f(1300.f, 450.f);
}

// Funzione per generare posizioni sicure per i nemici
std::vector<sf::Vector2f> generaPosizioniNemici(int numeroNemici, const Mappa& mappa, const Vector2f& playerPos) {
    std::vector<sf::Vector2f> posizioni;
    std::random_device rd;
    std::mt19937 gen(rd());

    // Area più a sinistra ora che il player è a destra
    std::uniform_real_distribution<float> distX(100.f, 1000.f);
    std::uniform_real_distribution<float> distY(50.f, 850.f);

    int tentativi = 0;
    while (posizioni.size() < numeroNemici && tentativi < 1000) {
        tentativi++;
        sf::Vector2f pos(distX(gen), distY(gen));

        // Controlla che non sia troppo vicino al player
        if (std::abs(pos.x - playerPos.x) < 200.f && std::abs(pos.y - playerPos.y) < 200.f) {
            continue;
        }

        // Controlla collisioni con ostacoli e muri
        if (!isPosizioneValida(pos, Vector2f(45.f, 45.f), mappa)) {
            continue;
        }

        posizioni.push_back(pos);
    }

    return posizioni;
}

int main() {
    RenderWindow window(VideoMode(1400, 900), "SFML con CMake - Difficile!");
    Clock clock;

    Mappa mappa(Vector2f(1400.f, 900.f));

    // Trova una posizione di spawn sicura per il player
    Vector2f playerSize(30.f, 30.f);
    Vector2f playerSpawnPos = trovaSpawnSicuroPlayer(mappa, playerSize);

    Personaggio player(playerSpawnPos, playerSize, 350.f);

    NavigationGrid navigazione(Vector2f(1400.f, 900.f), 40.f);
    navigazione.aggiornaOstacoli(mappa.getMuri(), mappa.getOstacoli());

    std::vector<Nemico> nemici;

    // Genera posizioni per i nemici usando la posizione effettiva del player
    auto posizioni = generaPosizioniNemici(8, mappa, player.getPosizione());

    if (posizioni.size() >= 8) {
        // 6 NEMICI PICCOLI E ULTRARAPIDI
        for (int i = 0; i < 6; i++) {
            nemici.push_back(Nemico(posizioni[i], Vector2f(25.f, 25.f), 480.f)); // Molto veloci
        }

        // 2 NEMICI GRANDI E RAPIDI
        for (int i = 6; i < 8; i++) {
            nemici.push_back(Nemico(posizioni[i], Vector2f(50.f, 50.f), 380.f)); // Veloci
        }
    } else {
        // Fallback se non riesce a generare posizioni - TUTTI ULTRA-RAPIDI
        // Usa la funzione isPosizioneValida per trovare posizioni sicure
        std::vector<Vector2f> fallbackPositions = {
            Vector2f(200.f, 200.f), Vector2f(400.f, 600.f), Vector2f(700.f, 400.f),
            Vector2f(300.f, 500.f), Vector2f(600.f, 300.f), Vector2f(800.f, 700.f),
            Vector2f(500.f, 300.f), Vector2f(900.f, 100.f)
        };

        for (const auto& pos : fallbackPositions) {
            Vector2f safePos = pos;
            Vector2f nemicoSize = (&pos == &fallbackPositions[6] || &pos == &fallbackPositions[7]) ?
                                 Vector2f(50.f, 50.f) : Vector2f(25.f, 25.f);

            // Se la posizione non è valida, cerca una vicina che lo sia
            if (!isPosizioneValida(pos, nemicoSize, mappa)) {
                bool trovata = false;
                for (float offset = 50.f; offset <= 200.f && !trovata; offset += 50.f) {
                    for (float dx = -offset; dx <= offset && !trovata; dx += 50.f) {
                        for (float dy = -offset; dy <= offset && !trovata; dy += 50.f) {
                            Vector2f newPos(pos.x + dx, pos.y + dy);
                            if (isPosizioneValida(newPos, nemicoSize, mappa) &&
                                std::abs(newPos.x - player.getPosizione().x) > 200.f) {
                                safePos = newPos;
                                trovata = true;
                                break;
                                }
                        }
                    }
                }
            }

            float speed = (&pos == &fallbackPositions[6] || &pos == &fallbackPositions[7]) ? 380.f : 480.f;
            nemici.push_back(Nemico(safePos, nemicoSize, speed));
        }
    }

    FloatRect mappaBounds(0.f, 0.f, 1400.f, 900.f);

    float apertura = 200.f;
    float centro = mappaBounds.height / 2.f;
    float yTop = centro - apertura / 2.f;
    float yBottom = centro + apertura / 2.f;

    // Timer per il game over
    float tempoTrascorso = 0.f;
    const float tempoMassimo = 60.f; // 60 secondi per vincere
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Fallback font
        font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    }
    Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::White);
    timerText.setPosition(10.f, 10.f);

    while (window.isOpen()) {
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();
        tempoTrascorso += deltaTime;
        float tempoRimanente = tempoMassimo - tempoTrascorso;

        // Aggiorna il timer
        timerText.setString("Tempo: " + std::to_string(static_cast<int>(tempoRimanente)) + "s");

        // Game over se il tempo scade
        if (tempoRimanente <= 0) {
            window.close();
            return 0;
        }

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

        // Vittoria se raggiungi l'uscita a SINISTRA
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

        // Aggiornamento dei nemici con controllo collisioni
        std::vector<Nemico*> puntatoriNemici;
        for (auto& nemico : nemici) {
            puntatoriNemici.push_back(&nemico);
        }

        for (auto& nemico : nemici) {
            nemico.aggiorna(player.getPosizione(), navigazione,
                           mappa.getOstacoli(), puntatoriNemici, deltaTime);

            if (checkCollision(player.getGlobalBounds(), nemico.getGlobalBounds())) {
                window.close();
                return 0;
            }
        }

        window.clear();
        mappa.disegna(window);
        player.disegna(window);

        for (const auto& nemico : nemici) {
            nemico.disegna(window);
        }

        // Disegna il timer
        window.draw(timerText);

        window.display();
    }

    return 0;
}