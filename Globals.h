#ifndef GLOBALS_H
#define GLOBALS_H

// IMPORT HEADERS
#include "Pos.h";
#include "Vector.h";

// GLOBAL DEFINITIONS
#define FRAME_RATE 60

// GLOBAL VARIABLES
bool firstLoad = true;

// DEBUGGING TOGGLE
#define DEBUG false
#if DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif


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

#endif // GLOBALS_H