#include "ResultsScreen.h"


ResultScreen::ResultScreen() {
    font.loadFromFile("assets/fonts/Bisdak.ttf");
    resultText.setFont(font);
    resultText.setCharacterSize(48);
    resultText.setFillColor(sf::Color::White);

     if (!playAgainTexture.loadFromFile("assets/images/PlayAgain.png")) {
        std::cerr << "Failed to load PlayAgain.png\n";
    }
    playAgainSprite.setTexture(playAgainTexture);
    playAgainSprite.setScale(1.f, 1.f);
}   

void ResultScreen::draw(sf::RenderWindow& window, Player& player, Player& dealer) {
    std::string resultTitle;

    if (player.bust()) resultTitle = "Player Bust! Dealer Wins!";
    else if (dealer.bust()) resultTitle = "Dealer Bust! You Win!";
    else if (player.getScore() > dealer.getScore()) resultTitle = "You Win!";
    else if (player.getScore() < dealer.getScore()) resultTitle = "Dealer Wins!";
    else resultTitle = "Push! It's a Tie.";

    resultText.setString(resultTitle);

    sf::FloatRect tb = resultText.getLocalBounds();
    resultText.setOrigin(tb.width / 2.f, tb.height / 2.f);
    resultText.setPosition(window.getSize().x / 2.f, window.getSize().y / 3.f);

    window.draw(resultText);
}


void ResultScreen::handleClick(sf::Vector2f worldPos, GameSceneState& gameState, GameScene& currentScene) {
    if (playAgainSprite.getGlobalBounds().contains(worldPos)) {
        gameState = GameSceneState::WaitingForBet;
        playClickSound();
    }
}

void ResultScreen::drawOverlay(sf::RenderWindow& window,
                               const std::string& resultTitle,
                               int playerWins,
                               int dealerWins,
                               int balance)
{
    const sf::View& view = window.getView();
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f topLeft = viewCenter - (viewSize * 0.5f);

    // Dimmer
    sf::RectangleShape dimmer(viewSize);
    dimmer.setFillColor(sf::Color(0, 0, 0, 180));
    dimmer.setPosition(topLeft);
    window.draw(dimmer);

    // Box (50% width, 60% height)
    sf::Vector2f boxSize(viewSize.x * 0.5f, viewSize.y * 0.6f);
    sf::Vector2f boxCenter = viewCenter;
    sf::RectangleShape box(boxSize);
    box.setFillColor(sf::Color(40, 40, 40));
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(3.f);
    box.setOrigin(boxSize * 0.5f);
    box.setPosition(boxCenter);
    window.draw(box);

    // Title
    resultText.setString(resultTitle);
    resultText.setCharacterSize(static_cast<unsigned>(viewSize.y * 0.06f));
    resultText.setFillColor(sf::Color::Cyan);
    {
        sf::FloatRect tb = resultText.getLocalBounds();
        resultText.setOrigin(tb.width * 0.5f, tb.height * 0.5f);
    }
    resultText.setPosition(boxCenter.x, boxCenter.y - (boxSize.y * 0.5f) + (viewSize.y * 0.08f));
    window.draw(resultText);

    // Details
    sf::Text details;
    details.setFont(font);
    details.setString("Player Wins: " + std::to_string(playerWins) +
                      "\nDealer Wins: " + std::to_string(dealerWins) +
                      "\nBalance: $" + std::to_string(balance));
    details.setCharacterSize(static_cast<unsigned>(viewSize.y * 0.035f));
    details.setFillColor(sf::Color::White);
    {
        sf::FloatRect db = details.getLocalBounds();
        details.setOrigin(db.width * 0.5f, 0.f);
    }
    details.setPosition(boxCenter.x, boxCenter.y - (viewSize.y * 0.03f));
    window.draw(details);

    // Play Again button: scale by % of view height
    float targetBtnH = viewSize.y * 0.40f; // 25% of height
    float texH = static_cast<float>(playAgainSprite.getTexture()->getSize().y);
    float scale = targetBtnH / texH;
    playAgainSprite.setScale(scale, scale);

sf::FloatRect btnBounds = playAgainSprite.getGlobalBounds();
float padding = viewSize.x * 0.02f;

playAgainSprite.setPosition(
    boxCenter.x + (boxSize.x * 0.5f) - padding - (btnBounds.width * 0.75f),
    boxCenter.y + (boxSize.y * 0.5f) - padding - (btnBounds.height * 0.52f)
);

    window.draw(playAgainSprite);
}
