#include "Mappa.h"

Mappa::Mappa(const sf::Vector2f& dim) : dimensioni(dim), coloreOstacolo(150, 75, 0) {
    float spessore = 30.f; // spessore dei muri
    sf::Color coloreMuro(80, 80, 80); // grigio scuro

    // ===== MURI SOPRA e SOTTO =====
    sf::RectangleShape muroTop({dimensioni.x, spessore});
    muroTop.setPosition(0.f, 0.f);
    muroTop.setFillColor(coloreMuro);
    muri.push_back(muroTop);

    sf::RectangleShape muroBottom({dimensioni.x, spessore});
    muroBottom.setPosition(0.f, dimensioni.y - spessore);
    muroBottom.setFillColor(coloreMuro);
    muri.push_back(muroBottom);

    // ===== MURI DESTRA =====
    sf::RectangleShape muroRight({spessore, dimensioni.y});
    muroRight.setPosition(dimensioni.x - spessore, 0.f);
    muroRight.setFillColor(coloreMuro);
    muri.push_back(muroRight);

    // ===== MURO SINISTRO con USCITA =====
    float apertura = 200.f; // 🔹 altezza del buco (più grande = più uscita)
    float centro = dimensioni.y / 2.f;
    float yTop = centro - apertura / 2.f;
    float yBottom = centro + apertura / 2.f;

    // parte alta del muro sinistro
    sf::RectangleShape muroLeftTop({spessore, yTop});
    muroLeftTop.setPosition(0.f, 0.f);
    muroLeftTop.setFillColor(coloreMuro);
    muri.push_back(muroLeftTop);

    // parte bassa del muro sinistro
    sf::RectangleShape muroLeftBottom({spessore, dimensioni.y - yBottom});
    muroLeftBottom.setPosition(0.f, yBottom);
    muroLeftBottom.setFillColor(coloreMuro);
    muri.push_back(muroLeftBottom);

    // 🔹 Genera ostacoli randomici
    generaOstacoli(12); // 12 ostacoli random
}

void Mappa::generaOstacoli(int numeroOstacoli) {
    // Setup per numeri randomici
    std::random_device rd;
    std::mt19937 gen(rd());

    // Dimensioni degli ostacoli (min e max)
    std::uniform_real_distribution<float> dimDist(30.f, 80.f);

    // Posizioni (lasciamo un margine dai bordi)
    float margine = 50.f;
    std::uniform_real_distribution<float> xDist(margine, dimensioni.x - margine);
    std::uniform_real_distribution<float> yDist(margine, dimensioni.y - margine);

    for (int i = 0; i < numeroOstacoli; ++i) {
        float larghezza = dimDist(gen);
        float altezza = dimDist(gen);

        sf::RectangleShape ostacolo({larghezza, altezza});
        ostacolo.setPosition(xDist(gen), yDist(gen));
        ostacolo.setFillColor(coloreOstacolo);
        ostacolo.setOutlineThickness(2.f);
        ostacolo.setOutlineColor(sf::Color(100, 50, 0)); // Bordo più scuro

        ostacoli.push_back(ostacolo);
    }
}

void Mappa::disegna(sf::RenderWindow& window) const {
    // Sfondo azzurro chiaro per la mappa
    sf::RectangleShape sfondo(dimensioni);
    sfondo.setFillColor(sf::Color(100, 150, 255)); // 🔵 azzurro
    window.draw(sfondo);

    // 🔹 Disegno degli ostacoli
    for (const auto& ostacolo : ostacoli)
        window.draw(ostacolo);

    // Disegno dei muri
    for (const auto& muro : muri)
        window.draw(muro);
}

const std::vector<sf::RectangleShape>& Mappa::getMuri() const {
    return muri;
}

const std::vector<sf::RectangleShape>& Mappa::getOstacoli() const {
    return ostacoli;
}

sf::FloatRect Mappa::getBounds() const {
    return sf::FloatRect(0.f, 0.f, dimensioni.x, dimensioni.y);
}