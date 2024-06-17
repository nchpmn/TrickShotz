// TrickShotz
// A Physics-Based Puzzler
// Copyright Nathan Chapman / nchpmn
// Licenced under GNU GPL 3.0
// Other Code & Libraries used as noted
//
// PROTOTYPE ONLY

#define VERSION "v240617 DEV"


// LIBRARIES
#include <Arduboy2.h>
Arduboy2 a;
// Micro font courtesy of @filmote - BSD 3-Clause License
#include "src/Font3x5.h"
Font3x5 font3x5 = Font3x5();


// HEADER FILES
#include "Globals.h";
#include "Ball.h";
#include "Line.h";
#include "Goal.h";
#include "Levels.h";
#include "Graphics.h";


// PLACEHOLDER OBJECTS
Ball playerBall(0, 0, 0);
Goal levelGoal(0, 0, 0);
Line levelLines[MAX_LINES];
uint8_t numLines;
uint8_t currentLevel;


// PROGRAM FLOWCHART
// Game States:
//     Title <-----> Instructions
//       |
//       v
//     PlayGame
//       |
//       v
//     EndGame
//       |
//       v
//     Title
// 
// PlayGame States:
//     Load
//       |
//       v
//     Aim
//       |
//       +---> ResetLevel
//       |
//       v
//     Launch -----+
//       |         |
//       v         |
//     LevelWin   LevelLose
//       |            |
//       v            v
//     Load       ResetLevel

// FUNCTIONS - GAME STATE
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
    if (a.justPressed(DOWN_BUTTON)) {
        showVersion = !showVersion;
    }
    if (showVersion) {
        drawInstructionsVersion();
    }
}
void drawInstructions() {
    font3x5.setCursor(5,5);
    font3x5.print(F("INSTRUCTIONS\nL+R: Set Angle\nU/D: Set Power\nA: Launch!\nB (HOLD): Reset Level"));
}
void drawInstructionsVersion() {
    font3x5.setCursor(0,50);
    font3x5.print(F(VERSION));
}

// GameState::PlayGame
void playGame() {
    switch(levelState) {
        case LevelState::Load:
            if (currentLevel >= NUM_LEVELS) {
                gameState = GameState::EndGame; // If all levels are completed, move to EndGame
            } else {
                updateLevelLoad(&levels[currentLevel], playerBall, levelGoal, levelLines, numLines);
                levelState = LevelState::Aim;
            }
            break;
        case LevelState::ResetLevel:
            updateResetLevel();
            break;
        case LevelState::Aim:
            updateAim();
            drawAim();
            break;
        case LevelState::Launch:
            updateLaunch();
            drawLaunch();
            break;
        case LevelState::LevelWin:
            updateWinLevel();
            drawWinLevel();
            break;
        case LevelState::LevelLose:
            updateLoseLevel();
            drawLoseLevel();
            break;
    }
}

// GameState::EndGame
void updateEndScreen() {
    if (a.justPressed(A_BUTTON)) {
        currentLevel = 0;
        gameState = GameState::Title;
    }
}
void drawEndScreen() {
    font3x5.setCursor(60,32);
    font3x5.print("END OF THE GAME\nYOU WIN!");
}


// FUNCTIONS - LEVEL STATE
// LevelState::Load
void updateLevelLoad(const LevelData *level, Ball &ball, Goal &goal, Line *lines, uint8_t &numLines) {
    DEBUG_PRINTLN("\nBEGIN updateLevelLoad()");
    #if DEBUG
    printLevelData(level);
    #endif
    // Load number of lines
    numLines = pgm_read_byte(&(level->numLines));

    // Load line data
    for (uint8_t i = 0; i < numLines; ++i) {
        LineData lineData;
        memcpy_P(&lineData, &(level->lines[i]), sizeof(LineData));
        lines[i] = Line(lineData.x1, lineData.y1, lineData.x2, lineData.y2);
    }
    
    // Load ball data
    BallData ballData;
    memcpy_P(&ballData, &(level->ball), sizeof(BallData));
    ball.setPosition(ballData.x, ballData.y);
    ball.setRadius(ballData.radius);
    DEBUG_PRINT("\nData as loaded:\nBall:");
    DEBUG_PRINT(ball.getX());
    DEBUG_PRINT(", ");
    DEBUG_PRINT(ball.getY());
    DEBUG_PRINT(", ");
    DEBUG_PRINTLN(ball.getRadius());

    // Load goal data
    GoalData goalData;
    memcpy_P(&goalData, &(level->goal), sizeof(GoalData));
    goal.setPosition(goalData.x, goalData.y);
    goal.setRadius(goalData.radius);

    DEBUG_PRINTLN("END updateLevelLoad()");
}

void drawLevel() {
    playerBall.draw();
    levelGoal.draw();
    for (uint8_t i = 0; i < numLines; ++i) {
        levelLines[i].draw();
    }
}

void drawLevelUI() {
    const int uiAngleX = 20;
    const int uiPowerX = 70;
    uint8_t heldFrames = 0;
    // Draw the bottom UI bar and elements
    a.fillRect(0, 55, 128, 15, BLACK);
    a.drawLine(0, 55, 128, 55, WHITE);

    // Reset level at any time
    if (a.pressed(B_BUTTON)) {
        if (heldFrames == 70) {
            // reload level
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
        // font3x5.print(playerBall.launchAngle);
        // if (playerBall.launchAngle >= 100) {
        //     // 3 digits = 9 Characters
        //     a.drawRect(uiAngleX + (9 * 4), 58, 3, 3, WHITE);
        // } else if (currentBall.launchAngle >= 10) {
        //     // 2 digits = 8 Characters
        //     a.drawRect(uiAngleX + (8 * 4), 58, 3, 3, WHITE);
        // } else {
        //     // 1 digit = 7 Characters
        //     a.drawRect(uiAngleX + (7 * 4), 58, 3, 3, WHITE);
        // }

        // Draw Power
        font3x5.setCursor(uiPowerX, 57);
        font3x5.print(F("POWER:"));
        // for (int i = 0; i < currentBall.launchPower; i++) {
        // a.fillRect((uiPowerX + 25 + i*4), (62 - i), 3, i+1);
        // }
    }
    if (a.justReleased(B_BUTTON)) {
        heldFrames = 0;
    }
}


// LevelState::Reset
void updateResetLevel() {

}

// LevelState::Aim
void updateAim() {
    // Move playerBall (prototype only)
    static bool moveFast = true;
    if (a.justPressed(A_BUTTON)) { moveFast = !moveFast; }
    if (moveFast) {
        if (a.pressed(LEFT_BUTTON)) { playerBall.move(Vector(MOVE_SPEED * -1, 0)); };
        if (a.pressed(RIGHT_BUTTON)) { playerBall.move(Vector(MOVE_SPEED, 0)); };
        if (a.pressed(UP_BUTTON)) { playerBall.move(Vector(0, MOVE_SPEED * -1)); };
        if (a.pressed(DOWN_BUTTON)) { playerBall.move(Vector(0, MOVE_SPEED)); };
    } else {
        if (a.justPressed(LEFT_BUTTON)) { playerBall.move(Vector(MOVE_SPEED * -1, 0)); };
        if (a.justPressed(RIGHT_BUTTON)) { playerBall.move(Vector(MOVE_SPEED, 0)); };
        if (a.justPressed(UP_BUTTON)) { playerBall.move(Vector(0, MOVE_SPEED * -1)); };
        if (a.justPressed(DOWN_BUTTON)) { playerBall.move(Vector(0, MOVE_SPEED)); };
    }
    static bool gravityEnabled = false;
    if (a.justPressed(B_BUTTON)) { gravityEnabled = !gravityEnabled; }
    if (gravityEnabled) {
        playerBall.move(Vector(0,GRAVITY_SPEED));
    }

    // Detect collision ball-goal
    // To be moved to LevelState::Launch after prototype
    if (playerBall.collideGoal(levelGoal.getPos(), levelGoal.getRadius())) {
        font3x5.setCursor(70,25);
        font3x5.print("GOAL!");
        levelState = LevelState::LevelWin;
    }

    // Detect collision ball-lines
    // To be moved to LevelState::Launch after prototype
    for (uint8_t i = 0; i < numLines; ++i) {
        if (playerBall.collideLine(levelLines[i].getStartPos(), levelLines[i].getEndPos(), levelLines[i].getThick())) {
            font3x5.setCursor(0,25);
            font3x5.print("Collide!");
        }
    }
}
void drawAim() {
    drawLevel();
    drawLevelUI();
    // playerBall.drawAim();
}

// LevelState::Launch
void updateLaunch() {

}
void drawLaunch() {
}

// LevelState::Win
void updateWinLevel() {
    if (a.justPressed(A_BUTTON)) {
        currentLevel++;
        levelState = LevelState::Load;
    }
}
void drawWinLevel() {
    drawLevel();
    drawLevelUI();
    font3x5.setCursor(60,0);
    font3x5.print("WIN!");
}

// LevelState::Lose
void updateLoseLevel() {

}
void drawLoseLevel() {
}


// MAIN SETUP
void setup() {
    a.begin();
    #if DEBUG
    Serial.begin(9600); // Initialize Serial communication at 9600 baud rate
    #endif
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
            updateEndScreen();
            drawEndScreen();
            break;
    }

    a.display();
}