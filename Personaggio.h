#ifndef PERSONAGGIO_H
#define PERSONAGGIO_H

#include <SFML/Graphics.hpp>

class Personaggio {
private:
    sf::RectangleShape forma;
    sf::Vector2f posizione;
    sf::Vector2f dimensione;
    float velocita;

public:
    Personaggio(const sf::Vector2f& pos, const sf::Vector2f& dim, float vel = 300.f);
    void muovi(const sf::Vector2f& direzione, float deltaTime);
    void disegna(sf::RenderWindow& finestra) const;
    void setPosizione(const sf::Vector2f& nuovaPos);
    sf::Vector2f getPosizione() const;
    sf::Vector2f getDimensione() const;
    sf::FloatRect getGlobalBounds() const;
};

#endif