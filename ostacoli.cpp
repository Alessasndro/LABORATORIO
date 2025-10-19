// Ostacoli.cpp
#include "Ostacoli.h"

Ostacoli::Ostacoli(const sf::Vector2f& dimMappa, int numeroOstacoli)
    : dimensioniMappa(dimMappa), coloreOstacolo(150, 75, 0) { // Marrone
    generaOstacoli(numeroOstacoli);
}

void Ostacoli::generaOstacoli(int numeroOstacoli) {
    ostacoli.clear();

    // Setup per numeri randomici
    std::random_device rd;
    std::mt19937 gen(rd());

    // Dimensioni degli ostacoli (min e max)
    std::uniform_real_distribution<float> dimDist(30.f, 100.f);

    // Posizioni (lasciamo un margine dai bordi)
    float margine = 50.f;
    std::uniform_real_distribution<float> xDist(margine, dimensioniMappa.x - margine);
    std::uniform_real_distribution<float> yDist(margine, dimensioniMappa.y - margine);

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

void Ostacoli::disegna(sf::RenderWindow& window) const {
    for (const auto& ostacolo : ostacoli) {
        window.draw(ostacolo);
    }
}

const std::vector<sf::RectangleShape>& Ostacoli::getOstacoli() const {
    return ostacoli;
}