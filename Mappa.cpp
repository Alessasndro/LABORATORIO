#include "Mappa.h"
#include <cmath>

Mappa::Mappa(const sf::Vector2f& dim) : dimensioni(dim), coloreOstacolo(150, 75, 0) {
    float spessore = 30.f;
    sf::Color coloreMuro(80, 80, 80);

    sf::RectangleShape muroTop({dimensioni.x, spessore});
    muroTop.setPosition(0.f, 0.f);
    muroTop.setFillColor(coloreMuro);
    muri.push_back(muroTop);

    sf::RectangleShape muroBottom({dimensioni.x, spessore});
    muroBottom.setPosition(0.f, dimensioni.y - spessore);
    muroBottom.setFillColor(coloreMuro);
    muri.push_back(muroBottom);

    sf::RectangleShape muroRight({spessore, dimensioni.y});
    muroRight.setPosition(dimensioni.x - spessore, 0.f);
    muroRight.setFillColor(coloreMuro);
    muri.push_back(muroRight);

    float apertura = 200.f;
    float centro = dimensioni.y / 2.f;
    float yTop = centro - apertura / 2.f;
    float yBottom = centro + apertura / 2.f;

    sf::RectangleShape muroLeftTop({spessore, yTop});
    muroLeftTop.setPosition(0.f, 0.f);
    muroLeftTop.setFillColor(coloreMuro);
    muri.push_back(muroLeftTop);

    sf::RectangleShape muroLeftBottom({spessore, dimensioni.y - yBottom});
    muroLeftBottom.setPosition(0.f, yBottom);
    muroLeftBottom.setFillColor(coloreMuro);
    muri.push_back(muroLeftBottom);

    generaOstacoli(12);
}

void Mappa::generaOstacoli(int numeroOstacoli) {
    ostacoli.clear();

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dimDist(30.f, 100.f);

    float margine = 50.f;
    std::uniform_real_distribution<float> xDist(margine, dimensioni.x - margine);
    std::uniform_real_distribution<float> yDist(margine, dimensioni.y - margine);

    int tentativiMax = 1000;
    int ostacoliCreati = 0;

    while (ostacoliCreati < numeroOstacoli && tentativiMax > 0) {
        float larghezza = dimDist(gen);
        float altezza = dimDist(gen);
        float x = xDist(gen);
        float y = yDist(gen);

        sf::FloatRect nuovoOstacolo(x, y, larghezza, altezza);

        bool collisione = false;

        for (const auto& ostacoloEsistente : ostacoli) {
            sf::FloatRect boundsEsistente = ostacoloEsistente.getGlobalBounds();

            if (boundsEsistente.intersects(nuovoOstacolo)) {
                collisione = true;
                break;
            }
        }

        if (!collisione) {
            sf::RectangleShape ostacolo({larghezza, altezza});
            ostacolo.setPosition(x, y);
            ostacolo.setFillColor(coloreOstacolo);
            ostacolo.setOutlineThickness(2.f);
            ostacolo.setOutlineColor(sf::Color(100, 50, 0));

            ostacoli.push_back(ostacolo);
            ostacoliCreati++;
        }

        tentativiMax--;
    }
}

void Mappa::disegna(sf::RenderWindow& window) const {
    sf::RectangleShape sfondo(dimensioni);
    sfondo.setFillColor(sf::Color(100, 150, 255));
    window.draw(sfondo);

    for (const auto& ostacolo : ostacoli)
        window.draw(ostacolo);

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