#include "GameManager.h"

// --- Global Variable Definitions ---
sf::Sprite xSprite;
sf::Music starterMusic;
sf::Texture tableTexture; 

// Gameplay Globals
sf::Sprite resumeButtonSprite;
sf::Sprite settingsButtonSprite;
sf::Sprite exitButtonSprite;
sf::Sprite playButtonSprite;
sf::Sprite cogSprite;

sf::Sprite bet1Sprite;
sf::Sprite bet10Sprite;
sf::Sprite bet25Sprite;
sf::Sprite bet50Sprite;

sf::Sprite clearBetSprite;
sf::Texture clearBetTexture;
sf::Sprite placeBetSprite;
sf::Texture placeBetTexture;

// Textures that were previously in main or gameplay
sf::Texture resumeTexture;
sf::Texture settingsTexture;
sf::Texture exitTexture;
sf::Texture betTexture1, betTexture10, betTexture25, betTexture50;

sf::Font mainFont;
sf::Font sideFont;
bool showGameplayOverlay = false;
bool showGameplaySettingsOverlay = false;


// --- GameManager Implementation ---

GameManager::GameManager()
    : window(sf::VideoMode(1280, 720), "Babble's Blackjack", sf::Style::Resize | sf::Style::Close),
      game(player, dealer),
      dealerHidden(true),
      currentScene(GameScene::Starter),
      gameState(GameSceneState::WaitingForBet),
      showSettingsOverlay(false),
      showGameplayOverlay(false),
      showGameplaySettingsOverlay(false),
      cardManager()
{
    initializeAssets();
}

void GameManager::initializeAssets() {
    // 1. Load System Cursors
    if (arrowCursor.loadFromSystem(sf::Cursor::Arrow)) window.setMouseCursor(arrowCursor);
    handCursor.loadFromSystem(sf::Cursor::Hand);

    // 2. Load Table Background
    if (!tableTexture.loadFromFile("assets/images/GameplayScreen.png")) {
        std::cerr << "Failed to load GameplayScreen.png\n";
    }

    // 3. Initialize Music
    initializeStarterMusic();

    // 4. Initialize Specific Scene UI
    // This calls your existing function in Gameplay.cpp to load chip textures/etc
    initializeGameplayUI(window);

    // 5. Ensure "Clear" and "Place Bet" sprites are ready for dynamic positioning
    // We force the Origin to be easy to work with:
    
    // Clear Button: Origin Top-Left (0,0)
    clearBetSprite.setOrigin(0.f, 0.f);
    
    //  Bet Button: Origin Bottom-Right
    sf::FloatRect placeBounds = placeBetSprite.getLocalBounds();
    placeBetSprite.setOrigin(placeBounds.width, placeBounds.height);
}

void GameManager::run() {
    // The Main Game Loop
    while (window.isOpen()) {
        
        processEvents();
        update();
        render();
    }
}

void GameManager::update() {
    updateDynamicLayout();
}

void GameManager::updateDynamicLayout() {
    sf::Vector2u windowSize = window.getSize();

    // Get window dimensions
    float windowWidth = (float)window.getSize().x;
    float windowHeight = (float)window.getSize().y;

    // Define consistent padding for all edges
    const float edgePadding = 40.0f; 

    // This scales comfortably on any monitor.
    float margin = 0.05f * static_cast<float>(windowSize.x);

    // --- 1. Position Clear Button (Bottom-Left) ---
    // X: Just the margin
    // Y: Window Height - Button Height - Margin
    sf::FloatRect clearBounds = clearBetSprite.getGlobalBounds();
    clearBetSprite.setPosition(
        edgePadding, 
        windowHeight - edgePadding - clearBounds.height 
    );

    // --- 2. Position Place Bet Button (Bottom-Right) ---
    // X: Window Width - Margin (since origin is bottom-right)
    // Y: Window Height - Margin (since origin is bottom-right)
    sf::FloatRect placeBounds = placeBetSprite.getGlobalBounds();
    placeBetSprite.setOrigin(placeBounds.width, placeBounds.height);
    float paddingX = window.getSize().x * 0.2f; // 20% of width
    float paddingY = window.getSize().y * 0.17f; // 20% of height
    placeBetSprite.setPosition(
        window.getSize().x - paddingX,
        window.getSize().y - paddingY
    );
}
void GameManager::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        // Handle Close
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (showSettingsOverlay) {
            drawSettingsOverlay(window, showSettingsOverlay, event);
        }
 

        // Handle Resize (Update the View)
        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
            // Force layout update immediately
            updateDynamicLayout();

            positionActionButtons(window);
        }

        // Handle ESC Key (Overlay Toggling)
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            if (currentScene == GameScene::Gameplay &&
                (gameState == GameSceneState::WaitingForBet ||   
                gameState == GameSceneState::RoundActive || 
                gameState == GameSceneState::ShowResult)) {
                showGameplayOverlay = !showGameplayOverlay;   // toggle in‑game menu
            }
            else if (currentScene == GameScene::Starter) {
                showSettingsOverlay = !showSettingsOverlay;   // Starter still uses settings overlay
            }
        }


        // Handle Mouse Clicks
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            
            sf::View currentView = window.getView();
            
            sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), currentView); 
            if (clickCooldown.getElapsedTime().asMilliseconds() > 150) {
                    // --- RESULT SCREEN CLICKS ---
                    if (gameState == GameSceneState::ShowResult) {
                        resultScreen.handleClick(worldPos, gameState, currentScene);

                        if (gameState == GameSceneState::WaitingForBet) {
                            player.resetHand();
                            dealer.resetHand();
                            dealerHidden = true;
                            playClickSound();
                        }

                        clickCooldown.restart();
                        return; // stop further click handling
                    }
                // --- STARTER SCENE CLICKS ---
                if (currentScene == GameScene::Starter) {
                     // Check if settings overlay X is clicked
                    if (showSettingsOverlay && xSprite.getGlobalBounds().contains(worldPos)) {
                        playClickSound();
                        showSettingsOverlay = false;
                    }
                    // Check Play Button
                    else if (!showSettingsOverlay && playButtonSprite.getGlobalBounds().contains(worldPos)) {
                        playClickSound();
                        currentScene = GameScene::Gameplay;
                    }
                    // Check Settings Cog
                    else if (!showSettingsOverlay && cogSprite.getGlobalBounds().contains(worldPos)) {
                        playClickSound();
                        showSettingsOverlay = true;
                    }
                }
                
                // --- GAMEPLAY SCENE CLICKS ---
                else if (currentScene == GameScene::Gameplay) {
                    // Debug print is okay here
                    std::cout << "DEBUG: Overlay Flags -> Settings: " << showSettingsOverlay 
                  << ", Gameplay Menu: " << showGameplayOverlay << "\n";
                  

                  if (currentScene == GameScene::Gameplay && showSettingsOverlay) {
                        // Forward mouse events to settings overlay
                        drawSettingsOverlay(window, showSettingsOverlay, event);
                    }
                    // Check Settings Overlay X
                    if (showSettingsOverlay && xSprite.getGlobalBounds().contains(worldPos)) {
                         playClickSound();
                         showSettingsOverlay = false;
                    }

   
                    
                    
                    // 🛑 REPLACED EXTERNAL FUNCTION CALL WITH ITS LOGIC 🛑
                    // This fixes the showGameplayOverlay = false bug!
                    else if (showGameplayOverlay) { 
                        
                        if (resumeButtonSprite.getGlobalBounds().contains(worldPos)) {
                            playClickSound();
                            this->showGameplayOverlay = false; // ✅ FIX: Resumes game
                            std::cout << "Resume clicked — overlay closed\n";
                        }

                        if (settingsButtonSprite.getGlobalBounds().contains(worldPos)) {
                            playClickSound();
                            this->showGameplayOverlay = false; // ✅ FIX: Hides menu
                            this->showSettingsOverlay = true;  // Shows settings
                            std::cout << "Settings clicked — opening settings overlay\n";
                        }

                        if (exitButtonSprite.getGlobalBounds().contains(worldPos)) {
                            playClickSound();
                            this->currentScene = GameScene::Starter; // ✅ Returns to start
                            this->showGameplayOverlay = false; // ✅ FIX: Clears menu
                            std::cout << "Exit clicked — returning to Starter screen\n";
                        }
                    }

                    // 🛑 END OF REPLACED BLOCK 🛑
                    
                    // Check Betting Interaction
                    else if (gameState == GameSceneState::WaitingForBet) {
                        // Pass click to your chip logic
                        auto tryBet = [&](sf::Sprite& chip, int amount) {
                            if (chip.getGlobalBounds().contains(worldPos)) {
                                if (player.getBalance() >= amount) {
                                    playClickSound(); 
                                    player.addToBet(amount);
                                    player.deductBalance(amount);
                                    std::cout << "Bet placed: $" << amount << " | Current bet: $" << player.getCurrentBet() << "\n";
                                } else {
                                    std::cout << "Not enough balance for $" << amount << " chip\n";
                                }
                            }
                        };

                        tryBet(bet1Sprite, 1);
                        tryBet(bet10Sprite, 10);
                        tryBet(bet25Sprite, 25);
                        tryBet(bet50Sprite, 50);

                        if (clearBetSprite.getGlobalBounds().contains(worldPos)) {
                            playClickSound();
                            int refund = player.getCurrentBet();
                            player.updateBalance(refund);
                            player.resetBet();
                            std::cout << "Bet cleared. Refunded: $" << refund << "\n";
                        }
                        

                        if (placeBetSprite.getGlobalBounds().contains(worldPos)) {
                            playClickSound();
                            std::cout << "Place Bet clicked — starting round\n";

                            if (player.getCurrentBet() > 0) {
                                std::cout << "Game state set to RoundActive\n";
                                startRound();
                                gameState = GameSceneState::RoundActive;
                            } else {
                                std::cout << "Cannot place bet — no chips selected\n";
                            }
                        }
                    }
                    else if (gameState == GameSceneState::RoundActive) {
                        if (getCenteredHitbox(hitButtonSprite, window, 0.08f, 0.05f).contains(worldPos)) {
                            playClickSound();
                            handleHit();
                        }
                        if (getCenteredHitbox(standButtonSprite, window, 0.08f, 0.05f).contains(worldPos)) {
                            playClickSound();
                            handleStand();
                        }
                        
                    }
                 }
                
                clickCooldown.restart();
            }
        }
    }
}

void GameManager::render() {
    window.clear();

    sf::Event dummyEvent; // Some of your draw functions ask for 'event', we pass a dummy if needed
    // Draw the main scenes
if (currentScene == GameScene::Starter) {
        renderSceneWithOverlay(window, currentScene, showSettingsOverlay, dummyEvent, player, dealer, gameState);
    }



    
    
    // Draw Gameplay specific UI
else if (currentScene == GameScene::Gameplay) {
        drawGameplayScreen(window, tableTexture, player, dealer, gameState, dealerHidden);

        // 👇 Call in‑game menu overlay here
        if (showGameplayOverlay) {
            drawInGameMenuOverlay(window, showGameplayOverlay, showSettingsOverlay, currentScene, dummyEvent);
        }

        if (showSettingsOverlay) {
            drawSettingsOverlay(window, showSettingsOverlay, dummyEvent);
        }
    }

       if (gameState == GameSceneState::ShowResult) {
    std::string resultTitle;

    if (player.bust()) resultTitle = "Dealer Wins!";
    else if (dealer.bust()) resultTitle = "You Win!";
    else if (player.getScore() > dealer.getScore()) resultTitle = "You Win!";
    else if (player.getScore() < dealer.getScore()) resultTitle = "Dealer Wins!";
    else resultTitle = "Push!";

    // ✅ Create a text object here
    sf::Text resultText;
    resultText.setFont(mainFont); // use your loaded font
    resultText.setString(resultTitle);
    resultText.setCharacterSize(48); // pick a size that looks good
    resultText.setFillColor(sf::Color::Cyan);

    // ✅ Center the text block (handles multi-line)
    sf::FloatRect tb = resultText.getLocalBounds();
    resultText.setOrigin(tb.left + tb.width / 2.f,
                         tb.top  + tb.height / 2.f);

    // ✅ Position in the middle of the window
    sf::Vector2u winSize = window.getSize();
    resultText.setPosition(winSize.x / 2.f, winSize.y / 4.f);

    window.draw(resultText);

    // Draw the rest of your overlay box/stats
    resultScreen.drawOverlay(window, resultTitle,
                             player.getWins(),
                             dealer.getWins(),
                             player.getBalance());
}
         
        
    // Draw Cursor Logic (Hand vs Arrow)
    if (currentScene == GameScene::Starter) {
        std::vector<sf::Sprite*> hoverTargets = { &playButtonSprite, &cogSprite, &xSprite };
        updateCursorForHoverTargets(window, hoverTargets, handCursor, arrowCursor);
    } else {
        window.setMouseCursor(arrowCursor);
    }

    window.display();
}

void GameManager::startRound() {
    player.resetHand();
    dealer.resetHand();

    player.addCard(game.dealCard());
    player.addCard(game.dealCard());
    dealer.addCard(game.dealCard());
    dealer.addCard(game.dealCard());

    dealerHidden = true;
    gameState = GameSceneState::RoundActive;
}


void GameManager::handleHit() {
    game.playerHit(player);
    if (player.bust()) {
        dealerHidden = false;
        gameState = GameSceneState::ShowResult;
    }
}

void GameManager::handleStand() {
    dealerHidden = false;
    game.dealerPlay(dealer);
    game.resolveRound(player, dealer);
    gameState = GameSceneState::ShowResult;
}



