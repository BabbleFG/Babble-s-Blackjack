    #include <SFML/Graphics.hpp>
    #include <SFML/Audio.hpp>
    #include <iostream>
    #include "StarterScreen.h"
    #include "utils.h"
    #include "Gameplay.h"
    #include "SceneState.h"
    extern sf::Music starterMusic;
    extern sf::Sprite xSprite;
    extern sf::Texture tableTexture;
    extern bool dealerHidden;
    void playClickSound() {
        static sf::SoundBuffer buffer;
        static sf::Sound sound;
        static bool loaded = false;

        if (!loaded) {
            if (!buffer.loadFromFile("assets/audio/clickNoise.ogg")) {
                std::cerr << "❌ Failed to load clickNoise.ogg\n";
                return;
            }
            sound.setBuffer(buffer);
            loaded = true;
        }

        sound.play();
    }

void applyHoverZoom(sf::Sprite& sprite, const sf::RenderWindow& window, float zoomFactor, sf::Vector2f baseScale) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    bool isHovering = getCenteredHitbox(sprite, window, 0.08f, 0.05f).contains(worldPos);
    sf::Vector2f targetScale = isHovering
        ? sf::Vector2f(baseScale.x * zoomFactor, baseScale.y * zoomFactor)
        : baseScale;

    sprite.setScale(targetScale);
}

    void scaleAndCenterSprite(sf::Sprite& sprite, sf::RenderWindow& window) {
    const sf::Texture* texture = sprite.getTexture();
    if (!texture) {
        std::cerr << "❌ Sprite has no texture\n";
        return;
    }

    sf::Vector2u textureSize = texture->getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    float scale = std::min(scaleX, scaleY);

    sprite.setScale(scale, scale);
    sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
    sprite.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
}

void updateCursorForHoverTargets(sf::RenderWindow& window, const std::vector<sf::Sprite*>& hoverTargets, sf::Cursor& handCursor, sf::Cursor& arrowCursor) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    for (const auto& sprite : hoverTargets) {
        if (sprite->getGlobalBounds().contains(worldPos)) {
            window.setMouseCursor(handCursor);
            return;
        }
    }

    window.setMouseCursor(arrowCursor);
}

void initializeStarterMusic() {
    static bool musicStarted = false;
    if (!musicStarted) {
        if (starterMusic.openFromFile("assets/audio/Menutheme.ogg")) {
            starterMusic.setLoop(true);
            starterMusic.setVolume(50);
            starterMusic.play();
            musicStarted = true;
        } else {
            std::cerr << "❌ Failed to load starter music\n";
        }
    }
}

float getVolumeFromSlider(const sf::RectangleShape& bar, const sf::CircleShape& knob) {
    float minX = bar.getPosition().x;
    float knobX = knob.getPosition().x;
    return std::clamp((knobX - minX) / bar.getSize().x, 0.f, 1.f);
}



 void drawSettingsOverlay(sf::RenderWindow& window, bool& showSettingsOverlay, const sf::Event& event) {
    static bool draggingKnob = false;
    static bool knobInitialized = false;
    static sf::CircleShape sliderKnob(10.f);
    static sf::RectangleShape sliderBar(sf::Vector2f(200.f, 10.f));

    // 1. Dim the background
     sf::Vector2u windowSize = window.getSize();
    sf::RectangleShape dimmer(sf::Vector2f(windowSize.x, windowSize.y));
    dimmer.setFillColor(sf::Color(0, 0, 0, 150)); // semi-transparent black
    window.draw(dimmer);

    // 2. Settings box
    sf::RectangleShape settingsBox(sf::Vector2f(windowSize.x * 0.6f, windowSize.y * 0.6f));
    settingsBox.setFillColor(sf::Color(50, 50, 50)); // dark gray
    settingsBox.setOutlineColor(sf::Color::White);
    settingsBox.setOutlineThickness(2.f);
    settingsBox.setOrigin(settingsBox.getSize() / 2.f);
    settingsBox.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    window.draw(settingsBox);


    // 3. X button
    static sf::Texture xTexture;
    static bool xLoaded = false;
    if (!xLoaded) {
        if (xTexture.loadFromFile("assets/images/Xbutton.png")) {
            xSprite.setTexture(xTexture);
            sf::Vector2u size = xTexture.getSize();
            xSprite.setOrigin(size.x / 2.f, size.y / 2.f);
            xLoaded = true;
        } else {
            std::cerr << "❌ Failed to load Xbutton.png\n";
        }
    }

    float xScale = windowSize.x / 1280.f * 0.3f;
    applyHoverZoom(xSprite, window, 1.2f, {xScale, xScale});
    xSprite.setPosition(
        settingsBox.getPosition().x + settingsBox.getSize().x / 2.f - 30.f,
        settingsBox.getPosition().y - settingsBox.getSize().y / 2.f + 30.f
    );
    window.draw(xSprite);


    
    // 4. Volume slider
    sf::Vector2f sliderSize(200.f, 10.f);
    sliderBar.setSize(sliderSize);
    sliderBar.setPosition(
    windowSize.x / 2.f - sliderSize.x / 2.f,
    windowSize.y / 2.f + 50.f
    );

    if (!knobInitialized) {
    float minX = sliderBar.getPosition().x;
    float volumePercent = starterMusic.getVolume() / 100.f;
    sliderKnob.setPosition(
        minX + volumePercent * sliderBar.getSize().x,
        sliderBar.getPosition().y + sliderBar.getSize().y / 2.f
    );
    knobInitialized = true;
}
    sliderBar.setFillColor(sf::Color::White);

    sliderKnob.setOrigin(10.f, 10.f);
    sliderKnob.setFillColor(sf::Color::Red);


    

    // Handle dragging
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left &&
        sliderKnob.getGlobalBounds().contains(mouseWorldPos)) {
        draggingKnob = true;
    }

    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {
        draggingKnob = false;
    }

    if (draggingKnob) {
        float minX = sliderBar.getPosition().x;
        float maxX = minX + sliderBar.getSize().x;
        float mouseX = std::clamp(mouseWorldPos.x, minX, maxX);

        sliderKnob.setPosition(mouseX, sliderKnob.getPosition().y);

        float volumePercent = (mouseX - minX) / sliderBar.getSize().x;
        starterMusic.setVolume(volumePercent * 100.f);
    }

    //volume label
    static sf::Texture volumeLabelTexture;
    static sf::Sprite volumeLabelSprite;
    static bool volumeLabelLoaded = false;

    if (!volumeLabelLoaded) {
        if (volumeLabelTexture.loadFromFile("assets/images/Volume.png")) {
            volumeLabelSprite.setTexture(volumeLabelTexture);
            volumeLabelLoaded = true;
        } else {
            std::cerr << "❌ Failed to load Volume.png\n";
        }
    }

    // 5. Close logic
    bool isHoveringX = xSprite.getGlobalBounds().contains(mouseWorldPos);
    static sf::Clock overlayClock;
    if ((event.type == sf::Event::MouseButtonPressed &&
         event.mouseButton.button == sf::Mouse::Left &&
         isHoveringX) ||
        (event.type == sf::Event::KeyPressed &&
         event.key.code == sf::Keyboard::Escape)) {
        playClickSound();
        showSettingsOverlay = false;
        overlayClock.restart();
        knobInitialized = false;
    }
    // Scale if needed
    volumeLabelSprite.setScale(0.5f, 0.5f); // Adjust to fit your layout

    // Position above slider
    sf::Vector2f sliderPos = sliderBar.getPosition();
    float labelOffsetY = 40.f; // Distance above slider
volumeLabelSprite.setPosition(
    sliderBar.getPosition().x + sliderBar.getSize().x / 2.f - volumeLabelSprite.getGlobalBounds().width / 2.f,
    sliderBar.getPosition().y - volumeLabelSprite.getGlobalBounds().height - 10.f // 10px padding above
);

    window.draw(volumeLabelSprite);


    
    // Draw slider
    window.draw(sliderBar);
    window.draw(sliderKnob);
    window.draw(volumeLabelSprite);

}




   void updateVolumeSlider(sf::RenderWindow& window, sf::RectangleShape& bar, sf::CircleShape& knob, bool& draggingKnob) {
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
        knob.getGlobalBounds().contains(mouseWorldPos)) {
        draggingKnob = true;
    }

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        draggingKnob = false;
    }

    if (draggingKnob) {
        float minX = bar.getPosition().x;
        float maxX = minX + bar.getSize().x;
        float mouseX = std::clamp(mouseWorldPos.x, minX, maxX);

        knob.setPosition(mouseX, knob.getPosition().y);

        float volumePercent = getVolumeFromSlider(bar, knob);
        starterMusic.setVolume(volumePercent * 100.f);
    }
}

 void renderSceneWithOverlay(sf::RenderWindow& window, GameScene currentScene, bool showSettingsOverlay, const sf::Event& event, Player& player, Player& dealer, GameSceneState& gameState) {
    if (showSettingsOverlay) {
        drawStarterScreen(window, currentScene, showSettingsOverlay, event);
        drawSettingsOverlay(window, showSettingsOverlay, event);
    } else {
        switch (currentScene) {
            case GameScene::Starter:
                drawStarterScreen(window, currentScene, showSettingsOverlay, event);
                break;

                case GameScene::Gameplay:
                drawGameplayScreen(window, tableTexture, player, dealer, gameState, dealerHidden);            
                break;
          
        }
    }

}

sf::FloatRect getCenteredHitbox(const sf::Sprite& sprite,
                                const sf::RenderWindow& window,
                                float widthPercent,
                                float heightPercent) {
    sf::Vector2f pos = sprite.getPosition();
    float w = window.getSize().x * widthPercent;
    float h = window.getSize().y * heightPercent;

    return sf::FloatRect(
        pos.x - w / 2.f,
        pos.y - h / 2.f,
        w,
        h
    );
}

void drawInGameMenuOverlay(sf::RenderWindow& window,bool& showGameplayOverlay,bool& showSettingsOverlay,GameScene& currentScene,const sf::Event& event){
  sf::Vector2u windowSize = window.getSize();
  int buttonCount = 3;

    // Dim background
    sf::RectangleShape dimmer(sf::Vector2f(windowSize.x, windowSize.y));
    dimmer.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(dimmer);

    // Menu box (responsive)
    sf::Vector2f boxSize(windowSize.x * 0.3f, windowSize.y * 0.7f);
    sf::RectangleShape menuBox(boxSize);
    menuBox.setFillColor(sf::Color(50, 50, 50));
    menuBox.setOutlineColor(sf::Color::White);
    menuBox.setOutlineThickness(2.f);
    menuBox.setOrigin(boxSize / 2.f);
    menuBox.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    window.draw(menuBox);

// Calculate vertical spacing
float boxTopY = menuBox.getPosition().y - boxSize.y / 2.f;
float boxBottomY = menuBox.getPosition().y + boxSize.y / 2.f;

// Add padding so buttons don’t touch top/bottom
float verticalPadding = boxSize.y * 0.15f; // 15% padding
float usableHeight = boxSize.y - (2 * verticalPadding);

// Even spacing between buttons
float gap = usableHeight / (buttonCount - 1);

// Position buttons
resumeButtonSprite.setPosition(windowSize.x / 2.f, boxTopY + verticalPadding + gap * 0);
settingsButtonSprite.setPosition(windowSize.x / 2.f, boxTopY + verticalPadding + gap * 1);
exitButtonSprite.setPosition(windowSize.x / 2.f, boxTopY + verticalPadding + gap * 2);

// Draw them
window.draw(resumeButtonSprite);
window.draw(settingsButtonSprite);
window.draw(exitButtonSprite);

    // Handle clicks
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        if (resumeButtonSprite.getGlobalBounds().contains(mouseWorldPos)) {
            playClickSound();
            showGameplayOverlay = false;
        }
        else if (settingsButtonSprite.getGlobalBounds().contains(mouseWorldPos)) {
            playClickSound();
            showGameplayOverlay = false;
            showSettingsOverlay = true;
        }
        else if (exitButtonSprite.getGlobalBounds().contains(mouseWorldPos)) {
            playClickSound();
            currentScene = GameScene::Starter;
            showGameplayOverlay = false;
        }
    }
}