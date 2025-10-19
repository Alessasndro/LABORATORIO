#ifndef MAPPA_H
#define MAPPA_H

#include <SFML/Graphics.hpp>
#include <vector>

class Mappa {
private:
    sf::Vector2f dimensioni;
    std::vector<sf::RectangleShape> muri;

public:
    explicit Mappa(const sf::Vector2f& dim);
    void disegna(sf::RenderWindow& window) const;

    [[nodiscard]] sf::FloatRect getBounds() const { return {0.f, 0.f, dimensioni.x, dimensioni.y}; }
    [[nodiscard]] const std::vector<sf::RectangleShape>& getMuri() const { return muri; }
};

#endif // MAPPA_H
