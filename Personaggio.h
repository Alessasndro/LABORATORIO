#pragma once
#include <SFML/Graphics.hpp>

class Personaggio {
private:
    sf::Vector2f posizione;
    sf::Vector2f dimensione;
    sf::RectangleShape forma;  // invece di sprite e texture
    float velocita;

public:
    Personaggio(sf::Vector2f pos, sf::Vector2f dim, float vel = 200.f);

    void disegna(sf::RenderWindow& finestra);
    void muovi(const sf::Vector2f& direzione, float deltaTime);

    sf::Vector2f getPosizione() const;
    sf::Vector2f getDimensione() const;
    void setPosizione(const sf::Vector2f& nuovaPos);
};
