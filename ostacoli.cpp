// Ostacoli.cpp
#include "Ostacoli.h"
#include <cmath>

Ostacoli::Ostacoli(const sf::Vector2f& dimMappa, int numeroOstacoli)
    : dimensioniMappa(dimMappa), coloreOstacolo(150, 75, 0) {
    generaOstacoli(numeroOstacoli);
}

void Ostacoli::generaOstacoli(int numeroOstacoli) {
    ostacoli.clear();

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dimDist(30.f, 100.f);

    float margine = 50.f;
    std::uniform_real_distribution<float> xDist(margine, dimensioniMappa.x - margine);
    std::uniform_real_distribution<float> yDist(margine, dimensioniMappa.y - margine);

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

            float distanzaMinima = 20.f;
            sf::FloatRect boundsAllargato(
                boundsEsistente.left - distanzaMinima,
                boundsEsistente.top - distanzaMinima,
                boundsEsistente.width + 2 * distanzaMinima,
                boundsEsistente.height + 2 * distanzaMinima
            );

            if (boundsAllargato.intersects(nuovoOstacolo)) {
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

void Ostacoli::disegna(sf::RenderWindow& window) const {
    for (const auto& ostacolo : ostacoli) {
        window.draw(ostacolo);
    }
}

const std::vector<sf::RectangleShape>& Ostacoli::getOstacoli() const {
    return ostacoli;
}