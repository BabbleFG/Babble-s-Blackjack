#include "StarterScreen.h"
#include "Utils.h"
#include <SFML/Audio.hpp>
#include <iostream>
extern sf::Music starterMusic;
static sf::Clock clickCooldown;
static sf::Clock titleClock;
extern sf::Sprite playButtonSprite;
extern sf::Sprite cogSprite;
void drawStarterScreen(sf::RenderWindow& window, GameScene& currentScene, bool& showSettingsOverlay, const sf::Event& event){
    
    // Background
     static sf::Texture backgroundTexture;
    static sf::Sprite backgroundSprite;
    static bool backgroundLoaded = false;
    if (!backgroundLoaded) {
        if (backgroundTexture.loadFromFile("assets/images/StarterBlackjack.png")) {
            backgroundSprite.setTexture(backgroundTexture);
            backgroundLoaded = true;
        } else {
            std::cerr << "❌ Failed to load StarterBlackjack.png\n";
        }
    }
    if (backgroundLoaded) {
        scaleAndCenterSprite(backgroundSprite, window);
        window.draw(backgroundSprite);
    }

    if (backgroundSprite.getTexture()) {
        scaleAndCenterSprite(backgroundSprite, window);
    
        window.draw(backgroundSprite);
    }
// 3. Play button setup
static sf::Texture playButtonTexture;
static bool buttonLoaded = false;

if (!buttonLoaded) {
    if (playButtonTexture.loadFromFile("assets/images/play_button.png")) {
        playButtonSprite.setTexture(playButtonTexture);

        // Set origin to center of texture
        sf::Vector2u size = playButtonTexture.getSize();
        playButtonSprite.setOrigin(static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f);

        buttonLoaded = true;
    } else {
        std::cerr << "❌ Failed to load play_button.png\n";
    }
}

    float playButtonScale = 0.6f;
    playButtonSprite.setScale(playButtonScale, playButtonScale); 
    applyHoverZoom(playButtonSprite, window, 1.2f, {0.6f, 0.6f});           
    playButtonSprite.setPosition(window.getSize().x / 2.f, window.getSize().y * 0.7f);
    window.draw(playButtonSprite);

    //  Cogwheel setup
    sf::Vector2u windowSize = window.getSize();
    static sf::Texture cogTexture;
    static bool cogLoaded = false;
    if (!cogLoaded) {
        if (cogTexture.loadFromFile("assets/images/cogwheel.png")) {
            cogSprite.setTexture(cogTexture);
            sf::Vector2u cogSize = cogTexture.getSize();
            cogSprite.setOrigin(static_cast<float>(cogSize.x) / 2.f, static_cast<float>(cogSize.y) / 2.f);
            cogLoaded = true;
        } else {
            std::cerr << "❌ Failed to load cogwheel.png\n";
        }
    }

sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    bool isHoveringCog = cogSprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    float cogScale = 0.8f;
    cogSprite.setScale(isHoveringCog ? cogScale * 1.2f : cogScale, isHoveringCog ? cogScale * 1.2f : cogScale);
    cogSprite.setPosition(windowSize.x - 40.f, 40.f);
    window.draw(cogSprite);
    
sf::Clock overlayClock;
if (event.type == sf::Event::MouseButtonPressed &&
    event.mouseButton.button == sf::Mouse::Left) {

    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (clickCooldown.getElapsedTime().asMilliseconds() > 150) {
        if (playButtonSprite.getGlobalBounds().contains(worldPos)) {
            playClickSound();
            currentScene = GameScene::Gameplay;
            std::cout << "Play clicked\n";
        }

        if (cogSprite.getGlobalBounds().contains(worldPos)) {
            playClickSound();
            showSettingsOverlay = true;
            std::cout << "Cogwheel clicked\n";
        }

        clickCooldown.restart();
    }
}
}


