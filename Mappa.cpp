#include "Mappa.h"
#include <iostream>

Mappa::Mappa(const sf::Vector2f& dimensioneFinestra)
    : textureSfondoCaricata(false), textureMuriCaricata(false), textureOstacoliCaricata(false) {

    // Inizializza lo sfondo
    sfondo.setSize(dimensioneFinestra);
    sfondo.setPosition(0, 0);
    sfondo.setFillColor(sf::Color(100, 100, 100)); // Colore di fallback

    creaMuri(dimensioneFinestra);
    creaOstacoli();
}

void Mappa::creaMuri(const sf::Vector2f& dimensioneFinestra) {
    muri.clear();

    // Mura perimetrali
    float spessoreMuro = 30.f;

    // Muro superiore - BIANCO
    sf::RectangleShape muroSuperiore(sf::Vector2f(dimensioneFinestra.x, spessoreMuro));
    muroSuperiore.setPosition(0, 0);
    muroSuperiore.setFillColor(sf::Color::White);
    muri.push_back(muroSuperiore);

    // Muro inferiore - BIANCO
    sf::RectangleShape muroInferiore(sf::Vector2f(dimensioneFinestra.x, spessoreMuro));
    muroInferiore.setPosition(0, dimensioneFinestra.y - spessoreMuro);
    muroInferiore.setFillColor(sf::Color::White);
    muri.push_back(muroInferiore);

    // Muro sinistro (con apertura) - BIANCO
    float altezzaApertura = 200.f;
    float centroY = dimensioneFinestra.y / 2.f;
    float ySuperiore = centroY - altezzaApertura / 2.f;
    float yInferiore = centroY + altezzaApertura / 2.f;

    // Parte superiore del muro sinistro - BIANCO
    if (ySuperiore > 0) {
        sf::RectangleShape muroSinistroSuperiore(sf::Vector2f(spessoreMuro, ySuperiore));
        muroSinistroSuperiore.setPosition(0, 0);
        muroSinistroSuperiore.setFillColor(sf::Color::White);
        muri.push_back(muroSinistroSuperiore);
    }

    // Parte inferiore del muro sinistro - BIANCO
    if (yInferiore < dimensioneFinestra.y) {
        sf::RectangleShape muroSinistroInferiore(sf::Vector2f(spessoreMuro, dimensioneFinestra.y - yInferiore));
        muroSinistroInferiore.setPosition(0, yInferiore);
        muroSinistroInferiore.setFillColor(sf::Color::White);
        muri.push_back(muroSinistroInferiore);
    }

    // Muro destro - BIANCO
    sf::RectangleShape muroDestro(sf::Vector2f(spessoreMuro, dimensioneFinestra.y));
    muroDestro.setPosition(dimensioneFinestra.x - spessoreMuro, 0);
    muroDestro.setFillColor(sf::Color::White);
    muri.push_back(muroDestro);

    // Muri interni - BIANCO
    sf::RectangleShape muroInterno1(sf::Vector2f(300.f, spessoreMuro));
    muroInterno1.setPosition(200.f, 200.f);
    muroInterno1.setFillColor(sf::Color::White);
    muri.push_back(muroInterno1);

    sf::RectangleShape muroInterno2(sf::Vector2f(spessoreMuro, 250.f));
    muroInterno2.setPosition(500.f, 400.f);
    muroInterno2.setFillColor(sf::Color::White);
    muri.push_back(muroInterno2);

    // Applica texture ai muri se caricata
    if (textureMuriCaricata) {
        for (auto& muro : muri) {
            muro.setTexture(&textureMuri);
        }
    }
}

void Mappa::creaOstacoli() {
    ostacoli.clear();

    // Ostacoli vari
    std::vector<sf::Vector2f> posizioniOstacoli = {
        sf::Vector2f(300.f, 500.f), sf::Vector2f(700.f, 300.f),
        sf::Vector2f(900.f, 600.f), sf::Vector2f(400.f, 700.f),
        sf::Vector2f(1100.f, 200.f), sf::Vector2f(600.f, 100.f)
    };

    for (const auto& pos : posizioniOstacoli) {
        sf::RectangleShape ostacolo(sf::Vector2f(60.f, 60.f));
        ostacolo.setPosition(pos);
        ostacolo.setFillColor(sf::Color::Red);
        ostacoli.push_back(ostacolo);
    }

    // Applica texture agli ostacoli se caricata
    if (textureOstacoliCaricata) {
        for (auto& ostacolo : ostacoli) {
            ostacolo.setTexture(&textureOstacoli);
        }
    }
}

bool Mappa::caricaTextureSfondo(const std::string& percorsoFile) {
    if (textureSfondo.loadFromFile(percorsoFile)) {
        sfondo.setTexture(&textureSfondo);
        textureSfondoCaricata = true;

        // Rimuovi il colore di fallback
        sfondo.setFillColor(sf::Color::White);

        std::cout << "Texture sfondo caricata con successo: " << percorsoFile << std::endl;
        return true;
    } else {
        std::cout << "Errore nel caricamento texture sfondo: " << percorsoFile << std::endl;
        textureSfondoCaricata = false;
        return false;
    }
}

bool Mappa::caricaTextureMuri(const std::string& percorsoFile) {
    if (textureMuri.loadFromFile(percorsoFile)) {
        textureMuriCaricata = true;

        // Applica texture a tutti i muri
        for (auto& muro : muri) {
            muro.setTexture(&textureMuri);
            muro.setFillColor(sf::Color::White); // Rimuovi colore solido
        }

        std::cout << "Texture muri caricata con successo: " << percorsoFile << std::endl;
        return true;
    } else {
        std::cout << "Errore nel caricamento texture muri: " << percorsoFile << std::endl;
        textureMuriCaricata = false;
        return false;
    }
}

bool Mappa::caricaTextureOstacoli(const std::string& percorsoFile) {
    if (textureOstacoli.loadFromFile(percorsoFile)) {
        textureOstacoliCaricata = true;

        // Applica texture a tutti gli ostacoli
        for (auto& ostacolo : ostacoli) {
            ostacolo.setTexture(&textureOstacoli);
            ostacolo.setFillColor(sf::Color::White); // Rimuovi colore solido
        }

        std::cout << "Texture ostacoli caricata con successo: " << percorsoFile << std::endl;
        return true;
    } else {
        std::cout << "Errore nel caricamento texture ostacoli: " << percorsoFile << std::endl;
        textureOstacoliCaricata = false;
        return false;
    }
}

void Mappa::disegna(sf::RenderWindow& window) const {
    // Disegna prima lo sfondo
    window.draw(sfondo);

    // Poi i muri
    for (const auto& muro : muri) {
        window.draw(muro);
    }

    // Infine gli ostacoli
    for (const auto& ostacolo : ostacoli) {
        window.draw(ostacolo);
    }
}

const std::vector<sf::RectangleShape>& Mappa::getMuri() const {
    return muri;
}

const std::vector<sf::RectangleShape>& Mappa::getOstacoli() const {
    return ostacoli;
}