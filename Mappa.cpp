#include "Mappa.h"

Mappa::Mappa(const sf::Vector2f& dim) : dimensioni(dim) {
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
}

void Mappa::disegna(sf::RenderWindow& window) const {
    // Sfondo azzurro chiaro per la mappa
    sf::RectangleShape sfondo(dimensioni);
    sfondo.setFillColor(sf::Color(100, 150, 255)); // 🔵 azzurro
    window.draw(sfondo);

    // Disegno dei muri
    for (const auto& muro : muri)
        window.draw(muro);
}