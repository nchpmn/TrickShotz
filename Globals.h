#pragma once

#include "Ball.h"

// CONSTANTS
const uint8_t MAX_LEVELS = 5;
const uint8_t MAX_PLANKS = 5;
const uint8_t FRAME_RATE = 60;
const float GRAVITY = 0.05;
const float BOUNCE_FRICTION = 0.9;
const uint8_t OFFSCREEN_SECONDS = 1;
const uint8_t HELD_FRAMES_DELAY = 45;
const uint8_t HELD_FRAMES_FREQ = 3;


// GLOBAL VARIABLES
Ball currentBall;
Goal currentGoal;
Plank currentPlanks[MAX_PLANKS];
uint16_t heldFrames = 0;
uint8_t currentLevel = 0;


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