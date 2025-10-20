#ifndef MAPPA_H
#define MAPPA_H

#include <SFML/Graphics.hpp>
#include <vector>

class Mappa {
private:
    std::vector<sf::RectangleShape> muri;
    std::vector<sf::RectangleShape> ostacoli;

    // Texture per la mappa
    sf::Texture textureSfondo;
    sf::RectangleShape sfondo;
    bool textureSfondoCaricata;

    // Texture per muri (opzionale)
    sf::Texture textureMuri;
    bool textureMuriCaricata;

    // Texture per ostacoli (opzionale)
    sf::Texture textureOstacoli;
    bool textureOstacoliCaricata;

    void creaMuri(const sf::Vector2f& dimensioneFinestra);
    void creaOstacoli();

public:
    Mappa(const sf::Vector2f& dimensioneFinestra);

    // Metodi per caricare texture
    bool caricaTextureSfondo(const std::string& percorsoFile);
    bool caricaTextureMuri(const std::string& percorsoFile);
    bool caricaTextureOstacoli(const std::string& percorsoFile);

    // Metodi esistenti
    void disegna(sf::RenderWindow& window) const;
    const std::vector<sf::RectangleShape>& getMuri() const;
    const std::vector<sf::RectangleShape>& getOstacoli() const;
};

#endif