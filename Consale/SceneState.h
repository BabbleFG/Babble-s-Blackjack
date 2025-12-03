#pragma once

enum class GameScene {
    Starter,
    Gameplay,
    results,
    SettingsOverlay
};


enum class GameSceneState {
    WaitingForBet,
    AnimatingChips,
    RoundActive, 
    DealingInitialCards,
    PlayerTurn,
    DealerTurn,
    ShowResult,
    Cleanup
};