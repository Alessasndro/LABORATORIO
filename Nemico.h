#ifndef NEMICO_H
#define NEMICO_H

#include <SFML/Graphics.hpp>
#include "NavigationGrind.h"
#include <vector>

class Nemico {
private:
    sf::RectangleShape forma;
    float velocitaMovimento;
    sf::Texture texture;
    bool textureCaricata;
    sf::Color colore;

    std::vector<sf::Vector2f> percorso;
    int waypointCorrente;
    float timerRicerca;

    bool checkCollisioneConOstacoli(const std::vector<sf::RectangleShape>& ostacoli, const sf::Vector2f& nuovaPosizione) const;
    bool checkCollisioneConNemici(const std::vector<Nemico*>& altriNemici, const sf::Vector2f& nuovaPosizione) const;

public:
    Nemico(const sf::Vector2f& posizione, const sf::Vector2f& dimensione, float velocita = 150.f);

    // Funzione per caricare la texture
    bool caricaTexture(const std::string& percorsoFile);

    void aggiorna(const sf::Vector2f& posizioneGiocatore, NavigationGrid& navigazione,
                 const std::vector<sf::RectangleShape>& ostacoli,
                 const std::vector<Nemico*>& altriNemici, float deltaTime);
    void disegna(sf::RenderWindow& window) const;
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getPosizione() const;
    sf::Vector2f getDimensione() const;
};

#endif