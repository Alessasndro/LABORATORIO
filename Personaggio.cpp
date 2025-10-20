#include "Personaggio.h"

Personaggio::Personaggio(const sf::Vector2f& pos, const sf::Vector2f& dim, float vel)
    : posizione(pos), dimensione(dim), velocita(vel) {

    forma.setSize(dimensione);
    forma.setPosition(posizione);
    forma.setFillColor(sf::Color::Green); // Colore di fallback
}

bool Personaggio::caricaTexture(const std::string& percorsoFile) {
    if (texture.loadFromFile(percorsoFile)) {
        forma.setTexture(&texture);
        forma.setFillColor(sf::Color::White); // Importante per visualizzare la texture
        return true;
    }
    return false;
}



sf::FloatRect Personaggio::getGlobalBounds() const {
    return forma.getGlobalBounds();
}

void Personaggio::disegna(sf::RenderWindow& finestra) const {
    finestra.draw(forma);
}

void Personaggio::muovi(const sf::Vector2f& direzione, float deltaTime) {
    sf::Vector2f nuovaPos = forma.getPosition() + direzione * velocita * deltaTime;
    forma.setPosition(nuovaPos);
}

sf::Vector2f Personaggio::getPosizione() const {
    return forma.getPosition();
}

sf::Vector2f Personaggio::getDimensione() const {
    return forma.getSize();
}

void Personaggio::setPosizione(const sf::Vector2f& nuovaPos) {
    forma.setPosition(nuovaPos);
}