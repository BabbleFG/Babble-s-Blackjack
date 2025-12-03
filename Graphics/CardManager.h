#ifndef CARDMANAGER_H
#define CARDMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "card.h"

class CardManager {
private:
    std::map<std::string, sf::Texture> cardTextures;
    sf::Texture backTexture;
    sf::Sprite backSprite;

public:
    CardManager() {
        preloadCards();
        backSprite.setTexture(backTexture);
    }

    void preloadCards() {
        std::string suits[4] = {"Clubs", "Diamonds", "Hearts", "Spades"};
        std::string ranks[13] = {"2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace"};

        for (auto& suit : suits) {
            for (auto& rank : ranks) {
                card c(suit, rank, 0);
                std::string filename = "assets/images/Cards/" + c.getAssetName() + ".png";
                sf::Texture texture;
                if (texture.loadFromFile(filename)) {
                    cardTextures[c.getAssetName()] = texture;
                } else {
                    std::cerr << "Failed to load " << filename << "\n";
                }
            }
        }

        backTexture.loadFromFile("assets/images/Cards/back.png");
    }

    sf::Sprite getSprite(const card& c) {
        sf::Sprite sprite;
        sprite.setTexture(cardTextures[c.getAssetName()]);
        return sprite;
    }

    const sf::Sprite& getBackSprite() const {
        return backSprite;
    }
};

#endif
