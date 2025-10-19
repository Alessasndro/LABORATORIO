#include "NavigationGrind.h"
#include <algorithm>
#include <iostream>

NavigationGrid::NavigationGrid(const sf::Vector2f& areaMappa, float dimensioneCella)
    : areaMappa(areaMappa), dimensioneCella(dimensioneCella) {

    int colonne = static_cast<int>(areaMappa.x / dimensioneCella);
    int righe = static_cast<int>(areaMappa.y / dimensioneCella);

    if (colonne > 0 && righe > 0) {
        grigliaOstacoli.resize(righe, std::vector<bool>(colonne, false));
    }
}

float NavigationGrid::calcolaDistanza(const sf::Vector2i& a, const sf::Vector2i& b) const {
    // Distanza euclidea per percorso più naturale
    float dx = static_cast<float>(a.x - b.x);
    float dy = static_cast<float>(a.y - b.y);
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<sf::Vector2i> NavigationGrid::getVicini(const sf::Vector2i& nodo) const {
    std::vector<sf::Vector2i> vicini;

    // 8 direzioni per movimento più fluido
    int direzioni[8][2] = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0},    // Cardinali
        {-1, -1}, {1, -1}, {1, 1}, {-1, 1}   // Diagonali
    };

    for (int i = 0; i < 8; ++i) {
        sf::Vector2i vicino(nodo.x + direzioni[i][0], nodo.y + direzioni[i][1]);
        if (eValido(vicino) && !grigliaOstacoli[vicino.y][vicino.x]) {
            vicini.push_back(vicino);
        }
    }

    return vicini;
}

bool NavigationGrid::eValido(const sf::Vector2i& coordinata) const {
    if (grigliaOstacoli.empty() || grigliaOstacoli[0].empty()) return false;
    return coordinata.x >= 0 && coordinata.y >= 0 &&
           coordinata.x < static_cast<int>(grigliaOstacoli[0].size()) &&
           coordinata.y < static_cast<int>(grigliaOstacoli.size());
}

void NavigationGrid::aggiungiMargineOstacoli() {
    // Crea una copia temporanea per evitare modifiche durante l'iterazione
    auto grigliaTemp = grigliaOstacoli;

    for (int y = 0; y < grigliaOstacoli.size(); ++y) {
        for (int x = 0; x < grigliaOstacoli[0].size(); ++x) {
            if (grigliaTemp[y][x]) {
                // Aggiungi margine attorno agli ostacoli
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < grigliaOstacoli[0].size() &&
                            ny >= 0 && ny < grigliaOstacoli.size()) {
                            grigliaOstacoli[ny][nx] = true;
                        }
                    }
                }
            }
        }
    }
}

void NavigationGrid::aggiornaOstacoli(const std::vector<sf::RectangleShape>& muri,
                                     const std::vector<sf::RectangleShape>& ostacoli) {
    // Reset griglia
    for (auto& riga : grigliaOstacoli) {
        std::fill(riga.begin(), riga.end(), false);
    }

    // Aggiungi muri con margine più ampio
    for (const auto& muro : muri) {
        sf::FloatRect bounds = muro.getGlobalBounds();
        int startX = std::max(0, static_cast<int>((bounds.left - 10) / dimensioneCella)); // Margine
        int startY = std::max(0, static_cast<int>((bounds.top - 10) / dimensioneCella));
        int endX = std::min(static_cast<int>(grigliaOstacoli[0].size()),
                           static_cast<int>((bounds.left + bounds.width + 10) / dimensioneCella));
        int endY = std::min(static_cast<int>(grigliaOstacoli.size()),
                           static_cast<int>((bounds.top + bounds.height + 10) / dimensioneCella));

        for (int y = startY; y < endY; ++y) {
            for (int x = startX; x < endX; ++x) {
                grigliaOstacoli[y][x] = true;
            }
        }
    }

    // Aggiungi ostacoli con margine
    for (const auto& ostacolo : ostacoli) {
        sf::FloatRect bounds = ostacolo.getGlobalBounds();
        int startX = std::max(0, static_cast<int>((bounds.left - 15) / dimensioneCella));
        int startY = std::max(0, static_cast<int>((bounds.top - 15) / dimensioneCella));
        int endX = std::min(static_cast<int>(grigliaOstacoli[0].size()),
                           static_cast<int>((bounds.left + bounds.width + 15) / dimensioneCella));
        int endY = std::min(static_cast<int>(grigliaOstacoli.size()),
                           static_cast<int>((bounds.top + bounds.height + 15) / dimensioneCella));

        for (int y = startY; y < endY; ++y) {
            for (int x = startX; x < endX; ++x) {
                grigliaOstacoli[y][x] = true;
            }
        }
    }

    // Aggiungi margine di sicurezza attorno a tutti gli ostacoli
    aggiungiMargineOstacoli();
}

std::vector<sf::Vector2f> NavigationGrid::calcolaPercorso(const sf::Vector2f& inizio, const sf::Vector2f& destinazione) {
    if (grigliaOstacoli.empty() || grigliaOstacoli[0].empty()) {
        return {inizio, destinazione};
    }

    sf::Vector2i startCoord(
        static_cast<int>(inizio.x / dimensioneCella),
        static_cast<int>(inizio.y / dimensioneCella)
    );
    sf::Vector2i targetCoord(
        static_cast<int>(destinazione.x / dimensioneCella),
        static_cast<int>(destinazione.y / dimensioneCella)
    );

    if (!eValido(startCoord) || !eValido(targetCoord)) {
        return {inizio, destinazione};
    }

    // Se la destinazione è un ostacolo, trova la cella libera più vicina
    if (grigliaOstacoli[targetCoord.y][targetCoord.x]) {
        // Cerca celle libere vicine alla destinazione
        for (int radius = 1; radius <= 3; ++radius) {
            for (int dy = -radius; dy <= radius; ++dy) {
                for (int dx = -radius; dx <= radius; ++dx) {
                    sf::Vector2i nuovoTarget(targetCoord.x + dx, targetCoord.y + dy);
                    if (eValido(nuovoTarget) && !grigliaOstacoli[nuovoTarget.y][nuovoTarget.x]) {
                        targetCoord = nuovoTarget;
                        radius = 4; // Break outer loop
                        break;
                    }
                }
            }
        }
    }

    auto comparatore = [](Node* a, Node* b) { return a->getFCosto() > b->getFCosto(); };
    std::priority_queue<Node*, std::vector<Node*>, decltype(comparatore)> listaAperta(comparatore);

    auto hash = [](const sf::Vector2i& v) { return v.x * 1812433253 + v.y; };
    std::unordered_map<sf::Vector2i, Node*, decltype(hash)> nodiAperti(10, hash);
    std::unordered_map<sf::Vector2i, Node*, decltype(hash)> nodiChiusi(10, hash);

    Node* startNode = new Node(startCoord);
    startNode->hCosto = calcolaDistanza(startCoord, targetCoord);
    listaAperta.push(startNode);
    nodiAperti[startCoord] = startNode;

    int iterazioniMax = 1000; // Limite per evitare loop infiniti
    int iterazioni = 0;

    while (!listaAperta.empty() && iterazioni < iterazioniMax) {
        iterazioni++;
        Node* nodoCorrente = listaAperta.top();
        listaAperta.pop();

        if (nodoCorrente->coordinata == targetCoord) {
            std::vector<sf::Vector2f> percorso;
            Node* nodo = nodoCorrente;
            while (nodo != nullptr) {
                percorso.push_back(sf::Vector2f(
                    nodo->coordinata.x * dimensioneCella + dimensioneCella / 2,
                    nodo->coordinata.y * dimensioneCella + dimensioneCella / 2
                ));
                nodo = nodo->genitore;
            }
            std::reverse(percorso.begin(), percorso.end());

            // Pulizia memoria
            for (auto& pair : nodiAperti) delete pair.second;
            for (auto& pair : nodiChiusi) delete pair.second;

            return percorso.size() > 1 ? percorso : std::vector<sf::Vector2f>{inizio, destinazione};
        }

        nodiAperti.erase(nodoCorrente->coordinata);
        nodiChiusi[nodoCorrente->coordinata] = nodoCorrente;

        for (const auto& vicinoCoord : getVicini(nodoCorrente->coordinata)) {
            if (nodiChiusi.count(vicinoCoord)) continue;

            float costoMovimento = (std::abs(vicinoCoord.x - nodoCorrente->coordinata.x) +
                                  std::abs(vicinoCoord.y - nodoCorrente->coordinata.y) == 2) ? 1.414f : 1.0f;
            float nuovoGCosto = nodoCorrente->gCosto + costoMovimento;

            if (!nodiAperti.count(vicinoCoord)) {
                Node* nodoVicino = new Node(vicinoCoord, nodoCorrente);
                nodoVicino->gCosto = nuovoGCosto;
                nodoVicino->hCosto = calcolaDistanza(vicinoCoord, targetCoord);
                listaAperta.push(nodoVicino);
                nodiAperti[vicinoCoord] = nodoVicino;
            } else {
                Node* nodoVicino = nodiAperti[vicinoCoord];
                if (nuovoGCosto < nodoVicino->gCosto) {
                    nodoVicino->gCosto = nuovoGCosto;
                    nodoVicino->genitore = nodoCorrente;
                }
            }
        }
    }

    // Pulizia memoria
    for (auto& pair : nodiAperti) delete pair.second;
    for (auto& pair : nodiChiusi) delete pair.second;

    return {inizio, destinazione};
}

bool NavigationGrid::isOstacolo(const sf::Vector2f& posizione) const {
    if (grigliaOstacoli.empty()) return false;

    int x = static_cast<int>(posizione.x / dimensioneCella);
    int y = static_cast<int>(posizione.y / dimensioneCella);

    if (x >= 0 && y >= 0 && x < grigliaOstacoli[0].size() && y < grigliaOstacoli.size()) {
        return grigliaOstacoli[y][x];
    }
    return true;
}