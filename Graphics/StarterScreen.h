#pragma once
#include <SFML/Graphics.hpp>
#include "SceneState.h"

void drawStarterScreen(sf::RenderWindow& window, GameScene& currentScene, bool& showSettingsOverlay, const sf::Event& event);
extern sf::Sprite playButtonSprite;
extern sf::Sprite cogSprite;

