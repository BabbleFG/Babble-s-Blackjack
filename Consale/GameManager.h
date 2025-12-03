#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>


#include "SceneState.h"
#include "player.h"
#include "game.h"
#include "utils.h"      
#include "Gameplay.h"   
#include "StarterScreen.h" 
#include "deck.h"
#include "CardManager.h"
#include "ResultsScreen.h"

class GameManager {
private:
    // --- Core Engine Components ---
    sf::RenderWindow window;
    GameScene currentScene;
    GameSceneState gameState;
    CardManager cardManager;
    ResultScreen resultScreen;
    
    // --- Game Logic Objects ---
    Player player;
    Player dealer;
    Game game;
    bool dealerHidden;
    Deck deck;

    // --- Input & Timing ---
    sf::Clock deltaClock;
    sf::Clock clickCooldown;
    sf::Cursor arrowCursor;
    sf::Cursor handCursor;

    // --- Resources ---
    sf::Texture tableTexture; 

    // --- State Flags ---
    bool showSettingsOverlay;
    bool showGameplayOverlay;
    bool showGameplaySettingsOverlay;

    // --- Private Methods ---
    void initializeAssets(); // Loads textures/audio
    void processEvents();    // Handle clicks, keyboard, resize
    void update();           // Handle game logic updates
    void render();           // Handle drawing to screen
    void updateDynamicLayout(); 

    

public:
    GameManager();
    void run();   
    void startRound();
    void handleHit();
    void handleStand();
    void hit();  
    void stand();
    sf::Sprite getScaledSprite(const card& c, const sf::RenderWindow& window);

};