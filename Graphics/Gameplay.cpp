#include "Gameplay.h"
#include "utils.h"
#include "GameManager.h"
#include "CardManager.h"
#include "game.h"

bool dealerHidden = true;
// preload textures once
static CardManager cardManager;   

// random card generator
static Deck deck;    

// --- Action Buttons ---
sf::Sprite hitButtonSprite;
sf::Sprite standButtonSprite;
sf::Texture hitButtonTexture;
sf::Texture standButtonTexture; 

//hands
static std::vector<card> dealerHand;
static std::vector<card> playerHand;


// --- Gameplay UI Elements ---
extern sf::Sprite resumeButtonSprite;
extern sf::Sprite settingsButtonSprite;
extern sf::Sprite exitButtonSprite;
extern sf::Texture resumeTexture;
extern sf::Texture settingsTexture;
extern sf::Texture exitTexture;



// --- Chip/Betting UI Elements ---
extern sf::Sprite bet1Sprite;
extern sf::Sprite bet10Sprite;
extern sf::Sprite bet25Sprite;
extern sf::Sprite bet50Sprite;
extern sf::Sprite clearBetSprite;
extern sf::Sprite placeBetSprite;
extern sf::Texture betTexture1;
extern sf::Texture betTexture10;
extern sf::Texture betTexture25;
extern sf::Texture betTexture50;
extern sf::Texture clearBetTexture;
extern sf::Texture placeBetTexture;

// --- Font ---
extern sf::Font mainFont;
extern sf::Font sideFont;
extern bool showGameplayOverlay;



void positionActionButtons(sf::RenderWindow& window) {
    float w = static_cast<float>(window.getSize().x);
    float h = static_cast<float>(window.getSize().y);

    float y = h * 0.90f;

    // Hit button: 95% of window width (right side)
    hitButtonSprite.setPosition(w * 0.70f, y);
    // Stand button: 85% of window width (to the left of Hit)
    standButtonSprite.setPosition(w * 0.87f, y);
}


void setOriginAfterScale(sf::Sprite& sprite) {
    sf::FloatRect localBounds = sprite.getLocalBounds();
    sprite.setOrigin(localBounds.width / 2.f, localBounds.height / 2.f);
}


void initializeGameplayUI(sf::RenderWindow& window) {

    // Load button textures
    resumeTexture.loadFromFile("assets/images/resume.png");
    settingsTexture.loadFromFile("assets/images/settings.png");
    exitTexture.loadFromFile("assets/images/menu.png");

    // Load font
    if (!mainFont.loadFromFile("assets/fonts/NeonTube.ttf")) {
        std::cerr << "Failed to load NeonTube.ttf\n";
    }

    if (!sideFont.loadFromFile("assets/fonts/Bisdak.ttf")) {
        std::cerr << "Failed to load Bisdak.ttf\n";
    }

    // Set button textures
    resumeButtonSprite.setTexture(resumeTexture);
    settingsButtonSprite.setTexture(settingsTexture);
    exitButtonSprite.setTexture(exitTexture);

    // Set button origins and scale
    float scaleFactor = window.getSize().x / 1280.f;
    resumeButtonSprite.setOrigin(resumeTexture.getSize().x / 2.f, resumeTexture.getSize().y / 2.f);
    settingsButtonSprite.setOrigin(settingsTexture.getSize().x / 2.f, settingsTexture.getSize().y / 2.f);
    exitButtonSprite.setOrigin(exitTexture.getSize().x / 2.f, exitTexture.getSize().y / 2.f);

    resumeButtonSprite.setScale(scaleFactor, scaleFactor);
    settingsButtonSprite.setScale(scaleFactor, scaleFactor);
    exitButtonSprite.setScale(scaleFactor, scaleFactor);

// Load chip textures
betTexture1.loadFromFile("assets/images/bet1.png");
betTexture10.loadFromFile("assets/images/bet10.png");
betTexture25.loadFromFile("assets/images/bet25.png");
betTexture50.loadFromFile("assets/images/bet50.png");

// Set chip textures
bet1Sprite.setTexture(betTexture1);
bet10Sprite.setTexture(betTexture10);
bet25Sprite.setTexture(betTexture25);
bet50Sprite.setTexture(betTexture50);

// Apply fixed scale5
float chipScale = 1.5f;
bet1Sprite.setScale(chipScale, chipScale);
bet10Sprite.setScale(chipScale, chipScale);
bet25Sprite.setScale(chipScale, chipScale);
bet50Sprite.setScale(chipScale, chipScale);


// Wait until textures are loaded and scaled before setting origin
auto setOriginAfterScale = [](sf::Sprite& sprite) {
    sf::FloatRect localBounds = sprite.getLocalBounds();
    sprite.setOrigin(
        localBounds.width / 2.f, 
        localBounds.height / 2.f
    );
};

setOriginAfterScale(bet1Sprite);
setOriginAfterScale(bet10Sprite);
setOriginAfterScale(bet25Sprite);
setOriginAfterScale(bet50Sprite);


float edgePadding = 15.f; 
float bottomPadding = 20.f;


// --- Clear/Reset Button (Bottom-Left) ---
clearBetTexture.loadFromFile("assets/images/clear.png"); 
clearBetSprite.setTexture(clearBetTexture);
clearBetSprite.setScale(0.3f, 0.3f);

clearBetSprite.setOrigin(0.f, 0.f);
sf::FloatRect clearBounds = clearBetSprite.getGlobalBounds();

// "Place Your Bet" button
placeBetTexture.loadFromFile("assets/images/placebet.png");
placeBetSprite.setTexture(placeBetTexture);

placeBetSprite.setScale(0.3f, 0.5f);

sf::FloatRect placeBounds = placeBetSprite.getLocalBounds();
placeBetSprite.setOrigin(placeBounds.width, placeBounds.height); // bottom-right origin



// Load Hit/Stand textures
hitButtonTexture.loadFromFile("assets/images/Hit.png");
standButtonTexture.loadFromFile("assets/images/Stand.png");

hitButtonSprite.setTexture(hitButtonTexture);
standButtonSprite.setTexture(standButtonTexture);

float scaleX = 0.6f; 
float scaleY = 0.8f; 
hitButtonSprite.setScale(scaleX, scaleY);
standButtonSprite.setScale(scaleX, scaleY);


setOriginAfterScale(hitButtonSprite);
setOriginAfterScale(standButtonSprite);

positionActionButtons(window);

}


void drawGameplayScreen(sf::RenderWindow& window,const sf::Texture& tableTexture,Player& player,Player& dealer,GameSceneState& gameState,bool dealerHidden){
  // 1. Draw table background
    sf::Sprite tableSprite;
    tableSprite.setTexture(tableTexture);
    sf::Vector2u textureSize = tableTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    tableSprite.setScale(
        static_cast<float>(windowSize.x) / textureSize.x,
        static_cast<float>(windowSize.y) / textureSize.y
    );
    window.draw(tableSprite);

    // 2. Draw gameplay overlay if active
    if (showGameplayOverlay) {
        sf::RectangleShape dimmer(sf::Vector2f(windowSize.x, windowSize.y));
        dimmer.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(dimmer);

        sf::Vector2f boxSize(windowSize.x * 0.4f, windowSize.y * 0.7f);
        sf::Vector2f boxCenter(windowSize.x / 2.f, windowSize.y / 2.f);
        sf::RectangleShape overlayBox(boxSize);
        overlayBox.setFillColor(sf::Color(40, 40, 40));
        overlayBox.setOutlineColor(sf::Color::White);
        overlayBox.setOutlineThickness(2.f);


        float centerX = boxCenter.x;
        float boxTopY = boxCenter.y - boxSize.y / 2.f;
        float buttonAreaHeight = boxSize.y;
        float gap = std::max(buttonAreaHeight / 4.f, 60.f);

        resumeButtonSprite.setPosition(centerX, boxTopY + gap - 20.f);
        settingsButtonSprite.setPosition(centerX, boxTopY + gap * 2);
        exitButtonSprite.setPosition(centerX, boxTopY + gap * 3);

        window.draw(resumeButtonSprite);
        window.draw(settingsButtonSprite);
        window.draw(exitButtonSprite);
    }

    // 3. Draw “Place Your Bet” overlay
    if (gameState == GameSceneState::WaitingForBet) {
    
        // Dim the background more
        sf::RectangleShape overlay(sf::Vector2f(windowSize.x, windowSize.y));
        overlay.setFillColor(sf::Color(0, 0, 0, 200)); // Increased opacity
        window.draw(overlay);

        float centerX = windowSize.x / 2.f;
    sf::Text betText; // ✅ Declare it here
    betText.setFont(mainFont);
    betText.setString("Your Bet: $" + std::to_string(player.getCurrentBet()));
    betText.setCharacterSize(36);
    betText.setFillColor(sf::Color::Yellow);
    betText.setOutlineColor(sf::Color::Black);
    betText.setOutlineThickness(2.f);

    sf::FloatRect betBounds = betText.getLocalBounds();
    betText.setOrigin(betBounds.width / 2.f, betBounds.height / 2.f);
    betText.setPosition(centerX, 370.f); // Just below balance
    window.draw(betText);
        // Prompt text
        sf::Text promptText;
        promptText.setFont(mainFont);
        promptText.setString("Place Your Bet");
        promptText.setCharacterSize(64); // Bigger
        promptText.setFillColor(sf::Color::Cyan);
        promptText.setOutlineColor(sf::Color::Blue);
        promptText.setOutlineThickness(3.f);

        sf::FloatRect promptBounds = promptText.getLocalBounds();
        promptText.setOrigin(promptBounds.width / 2.f, promptBounds.height / 2.f);
        promptText.setPosition(centerX, 240.f);
        window.draw(promptText);

        // Balance text
        sf::Text balanceText;
        balanceText.setFont(mainFont);
        balanceText.setString("Balance: $" + std::to_string(player.getBalance()));
        balanceText.setCharacterSize(40); // Bigger
        balanceText.setFillColor(sf::Color::White);

        sf::FloatRect balanceBounds = balanceText.getLocalBounds();
        balanceText.setOrigin(balanceBounds.width / 2.f, balanceBounds.height / 2.f);
        balanceText.setPosition(centerX, 310.f);
        window.draw(balanceText);

        float baseY = static_cast<float>(windowSize.y) - 100.f; // 100px from bottom
        float spacing = 150.f;

        bet1Sprite.setPosition(centerX - spacing * 1.5f, baseY);
        bet10Sprite.setPosition(centerX - spacing * 0.5f, baseY);
        bet25Sprite.setPosition(centerX + spacing * 0.5f, baseY);
        bet50Sprite.setPosition(centerX + spacing * 1.5f, baseY);

        // Draw chip sprites
        window.draw(bet1Sprite);
        window.draw(bet10Sprite);
        window.draw(bet25Sprite);
        window.draw(bet50Sprite);
        window.draw(clearBetSprite);
        window.draw(placeBetSprite);
    }

    if(gameState == GameSceneState::RoundActive || gameState == GameSceneState::ShowResult) {
        sf::Vector2u winSize = window.getSize();
        float winW = static_cast<float>(winSize.x);
        float winH = static_cast<float>(winSize.y);
        drawCards(window, player, dealer, dealerHidden);
        applyHoverZoom(hitButtonSprite, window, 1.2f, {0.6f, 0.8f});
        applyHoverZoom(standButtonSprite, window, 1.2f, {0.6f, 0.8f});  

        drawHandValue(window, dealer, mainFont, "Dealer", false, dealerHidden);
        drawHandValue(window, player, mainFont, "Player", true);

     if (gameState == GameSceneState::RoundActive) {
                window.draw(hitButtonSprite);
                window.draw(standButtonSprite);
            }
    }
}

sf::Sprite getScaledSprite(const sf::Sprite& rawSprite, const sf::RenderWindow& window) {
    sf::Sprite sprite = rawSprite; // copy so we can scale safely

    
    float desiredWidth  = window.getSize().x * 0.05f; 
    float desiredHeight = window.getSize().y * 0.12f; 

    sf::Vector2u texSize = sprite.getTexture()->getSize();
    sprite.setScale(
        desiredWidth / texSize.x,
        desiredHeight / texSize.y
    );

    return sprite;
}



void drawCards(sf::RenderWindow& window, Player& player, Player& dealer, bool dealerHidden) {
    float spacing = window.getSize().x * 0.03f; // 3% of window width

    // --- Player hand ---
    int playerCount = player.getCardCount();
    if (playerCount > 0) {
        // Use a sample card to measure scaled width
        sf::Sprite sample = getScaledSprite(cardManager.getSprite(player.getHand()[0]), window);
        float cardWidth = sample.getGlobalBounds().width;

        // Center horizontally
        float startX = (window.getSize().x - (playerCount * cardWidth + (playerCount - 1) * spacing)) / 2;
        // Place near bottom (60% down the window)
        float y = window.getSize().y * 0.60f;

        for (int i = 0; i < playerCount; ++i) {
            sf::Sprite cardSprite = getScaledSprite(cardManager.getSprite(player.getHand()[i]), window);
            cardSprite.setPosition(startX + i * (cardWidth + spacing), y);
            window.draw(cardSprite);
        }
    }

    // --- Dealer hand ---
    int dealerCount = dealer.getCardCount();
    if (dealerCount > 0) {
        // Use a sample card to measure scaled width
        sf::Sprite sample = getScaledSprite(cardManager.getSprite(dealer.getHand()[0]), window);
        float cardWidth = sample.getGlobalBounds().width;

        // Center horizontally
        float startX = (window.getSize().x - (dealerCount * cardWidth + (dealerCount - 1) * spacing)) / 2;
        // Place near top (17% down the window)
        float y = window.getSize().y * 0.17f;

        for (int i = 0; i < dealerCount; ++i) {
            if (i == 0 && dealerHidden) {
                // Back card scaled with the same formula
                sf::Sprite back = getScaledSprite(cardManager.getBackSprite(), window);
                back.setPosition(startX, y);
                window.draw(back);
            } else {
                sf::Sprite cardSprite = getScaledSprite(cardManager.getSprite(dealer.getHand()[i]), window);
                cardSprite.setPosition(startX + i * (cardWidth + spacing), y);
                window.draw(cardSprite);
            }
        }
    }
}


void drawHandValue(sf::RenderWindow& window,
                   Player& player,
                   const sf::Font& font,
                   const std::string& label,
                   bool aboveCards,
                   bool dealerHidden)
{
    int score;
    if (label == "Dealer" && dealerHidden && player.getCardCount() > 0) {
        score = player.getHand()[0].getValue(); // only first card visible
    } else {
        score = player.getScore();
    }

    sf::Text valueText;
    valueText.setFont(font);
    valueText.setString(label + ": " + std::to_string(score));
    valueText.setCharacterSize(28);
    valueText.setFillColor(sf::Color::White);

    // Center horizontally
    valueText.setOrigin(valueText.getLocalBounds().width / 2.f, 0.f);

    // Anchor to window size
    sf::Vector2u winSize = window.getSize();
    float winW = static_cast<float>(winSize.x);
    float winH = static_cast<float>(winSize.y);

    if (aboveCards) {
        valueText.setPosition(winW * 0.5f, winH * 0.550f); // Player score
    } else {
        valueText.setPosition(winW * 0.5f, winH * 0.29f); // Dealer score
    }

    window.draw(valueText);
}