#pragma once
#include <SFML/Graphics.hpp>
#include "SceneState.h"
#include "player.h"
#include "game.h"
#include "utils.h"
#include "Gameplay.h"
#include "StarterScreen.h"
#include "deck.h"
#include "CardManager.h"

class ResultScreen {
public:
    ResultScreen();

    void draw(sf::RenderWindow& window, Player& player, Player& dealer);
    void handleClick(sf::Vector2f worldPos, GameSceneState& gameState, GameScene& currentScene);
    void drawOverlay(sf::RenderWindow& window,const std::string& resultTitle,int playerWins,int dealerWins,int balance);
    const sf::Text& getResultText() const { return resultText; }
    std::string getResultString() const { return resultText.getString(); }

private:
    sf::Font font;
    sf::Text resultText;
    sf::Texture playAgainTexture; 
    sf::Sprite playAgainSprite; 
};
