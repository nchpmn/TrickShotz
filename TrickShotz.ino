// TrickShotz
// A Physics-Based Puzzler
// Copyright Nathan Chapman / nchpmn
// Licenced under GNU GPL 3.0
//
// 01 Nov 2023 - Development Started
// 04 December 2023 - First Demo Released
// 18 Dec 2023 - Major Refactor Begun
// PROTOTYPE ONLY

#define VERSION "v231225 DEV"

// LIBRARIES
#include <Arduboy2.h>
Arduboy2 a;
#include <ArduboyTones.h>
#include <ArduboyTonesPitches.h>
ArduboyTones sound(a.audio.enabled);
// Micro font courtesy of @filmote - BSD 3-Clause License
#include "src/Font3x5.h"
Font3x5 font3x5 = Font3x5();

// HEADER FILES
#include "Globals.h"
#include "Vector.h"
#include "Pos.h"
#include "Graphics.h"
#include "Tunes.h"
#include "Levels.h"
#include "Ball.h"


// FUNCTIONS
// GameState::Title
void updateTitle() {
    if (a.justPressed(A_BUTTON)) {
        gameState = GameState::PlayGame;
    }
    if (a.justPressed(B_BUTTON)) {
        gameState = GameState::Instructions;
    }
}
void drawTitle() {
    Sprites::drawOverwrite(0, 0, title, 0);
    font3x5.setCursor(2,5);
    font3x5.print(F("Press B for Instructions"));

}

// GameState::Instructions
void updateInstructions() {
    if (a.justPressed(A_BUTTON) || a.justPressed(B_BUTTON)) {
        gameState = GameState::Title;
    }
    static bool showVersion = false;
    if (showVersion) {
        font3x5.setCursor(0,50);
        font3x5.print(F(VERSION));
    }
    if (a.justPressed(DOWN_BUTTON)) {
        showVersion = !showVersion;
    }
}
void drawInstructions() {
    font3x5.setCursor(5,5);
    font3x5.print(F("INSTRUCTIONS\nL+R: Set Angle\nU/D: Set Power\nA: Launch!\nB (HOLD): Reset Level"));
}

// GameState::PlayGame
void playGame() {
    switch(levelState) {
        case LevelState::Load:
            font3x5.setCursor(5,10);
            font3x5.print(F("Load Level"));
            break;
        case LevelState::ResetLevel:
            font3x5.setCursor(5,10);
            font3x5.print(F("Reset Level"));
            break;
        case LevelState::Aim:
            font3x5.setCursor(5,10);
            font3x5.print(F("Aiming"));
            break;
        case LevelState::Launch:
            font3x5.setCursor(5,10);
            font3x5.print(F("Ball Launched!"));
            break;
        case LevelState::LevelWin:
            font3x5.setCursor(5,10);
            font3x5.print(F("You Win!"));
            break;
        case LevelState::LevelLose:
            font3x5.setCursor(5,10);
            font3x5.print(F("You Lose! Ha ha!"));
            break;
    }
}

// GameState::EndGame
void drawEndScreen() {
    font3x5.setCursor(10,10);
    font3x5.print(F("Congratulations!\nYou Completed All Levels!"));
}


// MAIN SETUP
void setup() {
    a.begin();
    a.setFrameRate(FRAME_RATE);
    a.initRandomSeed();
    a.clear();
}

// MAIN LOOP
void loop() {
    if (!(a.nextFrame())) {
        return;
    }
    a.pollButtons();
    a.clear();

    switch(gameState) {
        case GameState::Title:
            updateTitle();
            drawTitle();
            break;
        case GameState::Instructions:
            updateInstructions();
            drawInstructions();
            break;
        case GameState::PlayGame:
            playGame();
            break;
        case GameState::EndGame:
            drawEndScreen();
            break;
    }

    a.display();
}