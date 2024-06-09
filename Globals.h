#pragma once

// GLOBAL DEFINITIONS
#define FRAME_RATE 60
#define MOVE_SPEED 1 // not needed after prototype


// GAME STATE
enum class GameState {
    Title,
    Instructions,
    PlayGame,
    EndGame
};
GameState gameState;


// LEVEL STATE
enum class LevelState {
    Load,
    ResetLevel,
    Aim,
    Launch,
    LevelWin,
    LevelLose
};
LevelState levelState;