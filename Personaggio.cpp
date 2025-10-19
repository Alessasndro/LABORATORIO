#include "Personaggio.h"

Personaggio::Personaggio(sf::Vector2f pos, sf::Vector2f dim, float vel)
    : posizione(pos), dimensione(dim), velocita(vel)
{
    forma.setSize(dimensione);
    forma.setPosition(posizione);
    forma.setFillColor(sf::Color::Green);
}
sf::FloatRect Personaggio::getGlobalBounds() const {
    return forma.getGlobalBounds();
}

void Personaggio::disegna(sf::RenderWindow& finestra) {
    finestra.draw(forma);
}

void Personaggio::muovi(const sf::Vector2f& direzione, float deltaTime) {
    posizione += direzione * velocita * deltaTime;
    forma.setPosition(posizione);
}

sf::Vector2f Personaggio::getPosizione() const {
    return posizione;
}

sf::Vector2f Personaggio::getDimensione() const {
    return dimensione;
}

void Personaggio::setPosizione(const sf::Vector2f& nuovaPos) {
    posizione = nuovaPos;
    forma.setPosition(posizione);
}
