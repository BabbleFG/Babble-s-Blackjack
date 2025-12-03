#pragma once
#include <SFML/Graphics.hpp>
#include "SceneState.h"
#include <vector>
#include <iostream>
#include "player.h"
#include "CardManager.h"
using namespace std;

extern sf::Sprite resumeButtonSprite;
extern sf::Sprite settingsButtonSprite;
extern sf::Sprite exitButtonSprite;
extern bool showGameplaySettingsOverlay;

extern sf::Sprite hitButtonSprite;
extern sf::Sprite standButtonSprite;
extern sf::Texture hitButtonTexture;
extern sf::Texture standButtonTexture;


// ✅ Declare the function that renders the gameplay scene
void drawGameplayScreen(sf::RenderWindow& window,const sf::Texture& tableTexture,Player& player,Player& dealer,GameSceneState& gameState,bool dealerHidden);
void initializeGameplayUI(sf::RenderWindow& window);
void drawGameplaySettingsOverlay(sf::RenderWindow& window, bool& showGameplaySettingsOverlay, const sf::Event& event);
void handleGameplayOverlayClick(sf::RenderWindow& window, GameScene& currentScene, bool& showGameplayOverlay, bool& showSettingsOverlay, Player& player);
void handleBetClick(sf::RenderWindow& window, Player& player, GameSceneState& gameState);
void drawCards(sf::RenderWindow& window, Player& player, Player& dealer, bool dealerHidden);
void positionActionButtons(sf::RenderWindow& window);
void drawHandValue(sf::RenderWindow& window,Player& player,const sf::Font& font,const std::string& label,bool aboveCards,bool dealerHidden = false);
extern bool dealerHidden;
