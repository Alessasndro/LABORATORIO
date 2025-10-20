#include "Nemico.h"
#include <cmath>
#include <iostream>

Nemico::Nemico(const sf::Vector2f& posizione, const sf::Vector2f& dimensione, float velocita)
    : velocitaMovimento(velocita), waypointCorrente(0), timerRicerca(0) {

    forma.setSize(dimensione);
    forma.setPosition(posizione);

    // Colori diversi in base alle dimensioni
    if (dimensione.x > 40.f) { // Nemici grandi
        forma.setFillColor(sf::Color(139, 0, 0)); // Rosso scuro
        forma.setOutlineColor(sf::Color(100, 0, 0));
    } else { // Nemici piccoli
        forma.setFillColor(sf::Color(255, 100, 100)); // Rosso chiaro
        forma.setOutlineColor(sf::Color(200, 50, 50));
    }

    forma.setOutlineThickness(2.f);
}
bool Nemico::caricaTexture(const std::string& percorsoFile) {
    if (texture.loadFromFile(percorsoFile)) {
        forma.setTexture(&texture);
        textureCaricata = true;

        // Rimuovi i colori di fallback se la texture è caricata
        forma.setFillColor(sf::Color::White);
        forma.setOutlineColor(sf::Color::Transparent);

        return true;
    } else {
        std::cout << "Texture nemico non trovata: " << percorsoFile << std::endl;
        textureCaricata = false;
        return false;
    }
}
bool Nemico::checkCollisioneConOstacoli(const std::vector<sf::RectangleShape>& ostacoli, const sf::Vector2f& nuovaPosizione) const {
    sf::FloatRect bounds(nuovaPosizione.x, nuovaPosizione.y, forma.getSize().x, forma.getSize().y);

    for (const auto& ostacolo : ostacoli) {
        if (bounds.intersects(ostacolo.getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

bool Nemico::checkCollisioneConNemici(const std::vector<Nemico*>& altriNemici, const sf::Vector2f& nuovaPosizione) const {
    sf::FloatRect bounds(nuovaPosizione.x, nuovaPosizione.y, forma.getSize().x, forma.getSize().y);

    for (const auto& altroNemico : altriNemici) {
        if (altroNemico != this && bounds.intersects(altroNemico->getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

void Nemico::aggiorna(const sf::Vector2f& posizioneGiocatore, NavigationGrid& navigazione,
                     const std::vector<sf::RectangleShape>& ostacoli,
                     const std::vector<Nemico*>& altriNemici, float deltaTime) {
    timerRicerca += deltaTime;

    // Ricerca percorso meno frequente ma più stabile
    if (timerRicerca >= 0.5f || percorso.empty()) {
        percorso = navigazione.calcolaPercorso(forma.getPosition(), posizioneGiocatore);
        waypointCorrente = 0;
        timerRicerca = 0;
    }

    // Movimento fluido verso il waypoint
    if (!percorso.empty() && waypointCorrente < percorso.size()) {
        sf::Vector2f posizioneCorrente = forma.getPosition();
        sf::Vector2f direzione = percorso[waypointCorrente] - posizioneCorrente;

        float distanza = std::sqrt(direzione.x * direzione.x + direzione.y * direzione.y);

        if (distanza > 20.f) { // Soglia più grande per evitare oscillazioni
            if (distanza > 0.f) {
                direzione /= distanza;
            }

            // Velocità costante senza adattamenti che causano tremolio
            sf::Vector2f nuovaPosizione = posizioneCorrente + direzione * velocitaMovimento * deltaTime;

            // Controllo collisioni
            if (!checkCollisioneConOstacoli(ostacoli, nuovaPosizione) &&
                !checkCollisioneConNemici(altriNemici, nuovaPosizione)) {
                forma.setPosition(nuovaPosizione);
            } else {
                // Se collisione, passa al prossimo waypoint invece di cercare nuovo percorso
                waypointCorrente++;
                if (waypointCorrente >= percorso.size()) {
                    percorso.clear();
                }
            }
        } else {
            // Passa al waypoint successivo solo quando abbastanza vicino
            waypointCorrente++;
        }
    } else if (percorso.empty()) {
        // Movimento diretto solo se molto vicino al player
        sf::Vector2f posizioneCorrente = forma.getPosition();
        sf::Vector2f direzione = posizioneGiocatore - posizioneCorrente;

        float distanza = std::sqrt(direzione.x * direzione.x + direzione.y * direzione.y);
        if (distanza > 0.f && distanza < 300.f) { // Solo se relativamente vicino
            direzione /= distanza;

            sf::Vector2f nuovaPosizione = posizioneCorrente + direzione * velocitaMovimento * deltaTime * 0.7f;

            if (!checkCollisioneConOstacoli(ostacoli, nuovaPosizione) &&
                !checkCollisioneConNemici(altriNemici, nuovaPosizione)) {
                forma.setPosition(nuovaPosizione);
            }
        } else {
            // Se troppo lontano, forza ricerca percorso
            timerRicerca = 1.0f;
        }
    }
}

void Nemico::disegna(sf::RenderWindow& window) const {
    window.draw(forma);
}

sf::FloatRect Nemico::getGlobalBounds() const {
    return forma.getGlobalBounds();
}

sf::Vector2f Nemico::getPosizione() const {
    return forma.getPosition();
}

sf::Vector2f Nemico::getDimensione() const {
    return forma.getSize();
}