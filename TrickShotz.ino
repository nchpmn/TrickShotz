// TrickShotz
// A Physics-Based Puzzler
// Copyright Nathan Chapman / nchpmn
// Licenced under GNU GPL 3.0
//
// 01 Nov 2023 - Development Started
// 04 December 2023 - First Demo Released
// 18 Dec 2023 - Major Refactor Begun
// PROTOTYPE ONLY

#define VERSION "v231218 DEV"

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
#include "Graphics.h"
#include "Tunes.h"
#include "Levels.h"
#include "Plank.h"
#include "Ball.h"
#include "Goal.h"

// STATE MACHINE SETUP
GameState gameState = GameState::Title;
LevelState levelState = LevelState::Load;

// GLOBAL VARIABLES
Ball currentBall;
Goal currentGoal;
Plank currentPlanks[MAX_PLANKS];
uint16_t heldFrames = 0;
uint8_t currentLevel = 0;

void loadLevel(uint8_t n) {
    // Reset all game objects using data from levels[n]
    currentBall = Ball(levels[n].levelBallData[0], levels[n].levelBallData[1]);
    currentGoal = Goal(levels[n].levelGoalData[0],levels[n].levelGoalData[1],levels[n].levelGoalData[2]);
    for (uint8_t i = 0; i < levels[n].numPlanks; i++) {
        currentPlanks[i] = Plank(levels[n].levelPlanksData[i][0], levels[n].levelPlanksData[i][1], levels[n].levelPlanksData[i][2], levels[n].levelPlanksData[i][3]);
    }
}

// FUNCTIONS
void drawObjects() {
    // Draw all physics/level objects to screen
    currentBall.draw();
    currentGoal.draw();
    for (int i = 0; i < MAX_PLANKS; i++) {
        currentPlanks[i].draw();
    }
}

void drawUI() {
    const int uiAngleX = 20;
    const int uiPowerX = 70;
    // Draw the bottom UI bar and elements
    a.fillRect(0, 55, 128, 15, BLACK);
    a.drawLine(0, 55, 128, 55, WHITE);

    // Reset level at any time
    if (a.pressed(B_BUTTON)) {
        if (heldFrames == 70) {
            levelState = LevelState::Reload;
        } else {
            heldFrames++;
            a.fillRect(0, 57, heldFrames*2, 8);
            font3x5.setCursor(3, 57);
            font3x5.setTextColor(BLACK);
            font3x5.print(F("RESTART LEVEL..."));
            font3x5.setTextColor(WHITE);
        }
    } else {
        // Draw Angle
        font3x5.setCursor(uiAngleX, 57);
        font3x5.print(F("ANGLE:"));
        font3x5.print(currentBall.launchAngle);
        if (currentBall.launchAngle >= 100) {
            // 3 digits = 9 Characters
            a.drawRect(uiAngleX + (9 * 4), 58, 3, 3, WHITE);
        } else if (currentBall.launchAngle >= 10) {
            // 2 digits = 8 Characters
            a.drawRect(uiAngleX + (8 * 4), 58, 3, 3, WHITE);
        } else {
            // 1 digit = 7 Characters
            a.drawRect(uiAngleX + (7 * 4), 58, 3, 3, WHITE);
        }

        // Draw Power
        font3x5.setCursor(uiPowerX, 57);
        font3x5.print(F("POWER:"));
            for (int i = 0; i < currentBall.launchPower; i++) {
            a.fillRect((uiPowerX + 25 + i*4), (62 - i), 3, i+1);
        }
    }
    if (a.justReleased(B_BUTTON)) {
        heldFrames = 0;
    }
}

void drawMessagesBanner() {
    a.drawLine(0, 10, 128, 10, WHITE);
    a.fillRect(0, 11, 128, 9, BLACK);
    a.drawLine(0, 20, 128, 20, WHITE);
}

void drawMessages() {
    switch(levelState) {
        case LevelState::LevelWin:
            drawMessagesBanner();
            font3x5.setCursor((64 - 6*4), 12);
            font3x5.print("LEVEL CLEAR!");
            break;

        case LevelState::LevelLose:
            drawMessagesBanner();
            font3x5.setCursor((64 - 7*4), 12);
            font3x5.print("OUT OF BOUNDS!");
    }
}

void launchAngleUp(bool up) {
    uint8_t adjustStep = 1;
    if (up) {
        if (currentBall.launchAngle < 359) {
            currentBall.launchAngle += adjustStep;
        } else {
            currentBall.launchAngle = 0.0;
        }
    } else {
        if (currentBall.launchAngle > 0) {
            currentBall.launchAngle -= adjustStep;
        } else {
            currentBall.launchAngle = 360 - adjustStep;
        }
    }
}

void playGame() {
    switch(levelState) {
        case LevelState::Load:
            loadLevel(currentLevel);
            levelState = LevelState::Aim;
            break;
        
        case LevelState::Aim:
            if (a.justPressed(A_BUTTON)) {
                currentBall.setVelocity();
                levelState = LevelState::Play;
            }

            // Set launchAngle (Left/Right)
            if (a.justPressed(RIGHT_BUTTON)) {
                launchAngleUp(true);
            } else if (a.pressed(RIGHT_BUTTON)) {
                if (heldFrames > HELD_FRAMES_DELAY && heldFrames % HELD_FRAMES_FREQ == 0) {
                    launchAngleUp(true);
                }
                heldFrames++;
            }
            if (a.justPressed(LEFT_BUTTON)) {
                launchAngleUp(false);
            }else if (a.pressed(LEFT_BUTTON)) {
                if (heldFrames > HELD_FRAMES_DELAY && heldFrames % HELD_FRAMES_FREQ == 0) {
                    launchAngleUp(false);
                }
                heldFrames++;
            }
            if (a.justReleased(RIGHT_BUTTON) || (a.justReleased(LEFT_BUTTON))) {
                heldFrames = 0;
            }

            // Set launchPower (Up/Down)
            if (a.justPressed(UP_BUTTON)) {
                if (currentBall.launchPower < 5) {
                    currentBall.launchPower += 1;
                }
            }
            if (a.justPressed(DOWN_BUTTON)) {
                if (currentBall.launchPower > 1) {
                    currentBall.launchPower -= 1;
                }
            }
            break;
        
        case LevelState::Play:
            if (currentGoal.isBallInside(currentBall)) {
                sound.tones(winSong);
                levelState = LevelState::LevelWin;
            }

            currentBall.update(currentPlanks, MAX_PLANKS);
            break;
        
        case LevelState::LevelWin:
            a.drawLine(0, 10, 128, 10, WHITE);
            a.fillRect(0, 11, 128, 7, BLACK);
            font3x5.setCursor((64 - 12*3), 12);
            font3x5.print("LEVEL CLEAR!");
            a.drawLine(0, 19, 128, 19, WHITE);

            if (a.justPressed(A_BUTTON)) {
                if (currentLevel == MAX_LEVELS) {
                    gameState = GameState::EndScreen;
                } else {
                    currentLevel++;
                    levelState = LevelState::Load;
                }
            }
            break;

        case LevelState::LevelLose:
            a.drawLine(0, 10, 128, 10, WHITE);
            a.fillRect(0, 11, 128, 7, BLACK);
            font3x5.setCursor((64 - 12*3), 12);
            font3x5.print("OUT OF BOUNDS!");
            a.drawLine(0, 19, 128, 19, WHITE);
            break;
        
        case LevelState::Reload:
            uint8_t restartAngle = currentBall.launchAngle;
            uint8_t restartPower = currentBall.launchPower;
            loadLevel(currentLevel);
            currentBall.launchAngle = restartAngle;
            currentBall.launchPower = restartPower;
            levelState = LevelState::Aim;
            break;
    }

    drawObjects();
    drawUI();
    drawMessages();

}

void levelLose() {
    levelState = LevelState::LevelLose;
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
            Sprites::drawOverwrite(0, 0, title, 0);
            if (a.justPressed(A_BUTTON)) {
                gameState = GameState::PlayGame;
            }
            if (a.justPressed(B_BUTTON)) {
                gameState = GameState::Instructions;
            }
            break;
        case GameState::Instructions:
            font3x5.setCursor(5,5);
            font3x5.print(F("Instructions\nL/R: Set Angle\nU/D: Set Power\nA: Launch!\nB: Reset Level"));
            if (a.justPressed(A_BUTTON)) {
                gameState = GameState::Title;
            }
            // Toggle showing version number
            static bool showVersion = false;
            if (showVersion) {
                font3x5.setCursor(0,50);
                font3x5.print(VERSION);
            }
            if (a.justPressed(DOWN_BUTTON)) {
                showVersion = !showVersion;
            }
            break;
        case GameState::PlayGame:
            playGame();
            break;
        case GameState::EndScreen:
            font3x5.print(F("Congratulations!\nYou Completed All Levels!"));
            break;
    }
    
    a.display();
}