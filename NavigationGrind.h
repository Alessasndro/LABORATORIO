#ifndef NAVIGATIONGRID_H
#define NAVIGATIONGRID_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <queue>
#include <unordered_map>
#include <cmath>

struct Node {
    sf::Vector2i coordinata;
    float gCosto;
    float hCosto;
    Node* genitore;

    Node(sf::Vector2i coord, Node* parent = nullptr)
        : coordinata(coord), gCosto(0), hCosto(0), genitore(parent) {}

    float getFCosto() const { return gCosto + hCosto; }

    bool operator>(const Node& other) const {
        return getFCosto() > other.getFCosto();
    }
};

class NavigationGrid {
private:
    sf::Vector2f areaMappa;
    float dimensioneCella;
    std::vector<std::vector<bool>> grigliaOstacoli;

    float calcolaDistanza(const sf::Vector2i& a, const sf::Vector2i& b) const;
    std::vector<sf::Vector2i> getVicini(const sf::Vector2i& nodo) const;
    bool eValido(const sf::Vector2i& coordinata) const;
    void aggiungiMargineOstacoli();

public:
    NavigationGrid(const sf::Vector2f& areaMappa, float dimensioneCella = 40.f); // Cella più piccola
    void aggiornaOstacoli(const std::vector<sf::RectangleShape>& muri,
                         const std::vector<sf::RectangleShape>& ostacoli);
    std::vector<sf::Vector2f> calcolaPercorso(const sf::Vector2f& inizio, const sf::Vector2f& destinazione);
    bool isOstacolo(const sf::Vector2f& posizione) const;
};

#endif