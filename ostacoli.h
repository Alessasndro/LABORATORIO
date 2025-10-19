// Ostacoli.h
#ifndef OSTACOLI_H
#define OSTACOLI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class Ostacoli {
private:
    std::vector<sf::RectangleShape> ostacoli;
    sf::Vector2f dimensioniMappa;
    sf::Color coloreOstacolo;

public:
    Ostacoli(const sf::Vector2f& dimMappa, int numeroOstacoli = 10);
    void generaOstacoli(int numeroOstacoli);
    void disegna(sf::RenderWindow& window) const;
    const std::vector<sf::RectangleShape>& getOstacoli() const;
};

#endif