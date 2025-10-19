#ifndef MAPPA_H
#define MAPPA_H

#include <SFML/Graphics.hpp>
#include <vector>

class Mappa {
private:
    sf::Vector2f dimensioni;
    std::vector<sf::RectangleShape> muri;
    std::vector<sf::RectangleShape> ostacoli;
    sf::Color coloreOstacolo;

    void generaOstacoli(int numeroOstacoli);

public:
    Mappa(const sf::Vector2f& dim);
    void disegna(sf::RenderWindow& window) const;
    const std::vector<sf::RectangleShape>& getMuri() const;
    const std::vector<sf::RectangleShape>& getOstacoli() const;
    sf::FloatRect getBounds() const;
};

#endif