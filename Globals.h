#ifndef GLOBALS_H
#define GLOBALS_H

// CONSTANTS
const uint8_t MAX_LEVELS = 5;
const uint8_t MAX_PLANKS = 5;
const uint8_t FRAME_RATE = 60;
const float GRAVITY = 0.05;
const float BOUNCE_FRICTION = 0.9;
const uint8_t OFFSCREEN_SECONDS = 1;
const uint8_t HELD_FRAMES_DELAY = 45;
const uint8_t HELD_FRAMES_FREQ = 3;


// LEVEL STATE
enum class LevelState {
    Load,
    Reload,
    Aim,
    Play,
    LevelWin,
    LevelLose
};
extern LevelState levelState;

// GAME STATE
enum class GameState {
    Title,
    Instructions,
    PlayGame,
    EndScreen
};
extern GameState gameState;

#endif