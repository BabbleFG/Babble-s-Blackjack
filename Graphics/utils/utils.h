#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include "SceneState.h"
#include "player.h"
extern sf::Music starterMusic;
void initializeStarterMusic();
void playClickSound();
void applyHoverZoom(sf::Sprite& sprite, const sf::RenderWindow& window, float zoomFactor, sf::Vector2f baseScale);
void scaleAndCenterSprite(sf::Sprite& sprite, sf::RenderWindow& window);
void drawSettingsOverlay(sf::RenderWindow& window, bool& showSettingsOverlay, const sf::Event& event);
void updateCursorForHoverTargets(sf::RenderWindow& window, const std::vector<sf::Sprite*>& hoverTargets, sf::Cursor& handCursor, sf::Cursor& arrowCursor);
float getVolumeFromSlider(const sf::RectangleShape& bar, const sf::CircleShape& knob);
void updateVolumeSlider(sf::RenderWindow& window, sf::RectangleShape& bar, sf::CircleShape& knob, bool& draggingKnob);
void renderSceneWithOverlay(sf::RenderWindow& window, GameScene currentScene, bool showSettingsOverlay, const sf::Event& event, Player& player, Player& dealer, GameSceneState& gameState);
sf::FloatRect getCenteredHitbox(const sf::Sprite& sprite, const sf::RenderWindow& window, float widthPercent, float heightPercent);
void drawInGameMenuOverlay(sf::RenderWindow& window,bool& showGameplayOverlay,bool& showSettingsOverlay,GameScene& currentScene,const sf::Event& event);

extern sf::Sprite xSprite;
