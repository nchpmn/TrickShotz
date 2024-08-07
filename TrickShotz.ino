// TrickShotz
// A Physics-Based Puzzler
// Copyright Nathan Chapman / nchpmn
// Licenced under GNU GPL 3.0
// Other Code & Libraries used as noted
//
// PROTOTYPE ONLY

#define VERSION "v240707 DEV"


// LIBRARIES
// Arduboy2 library courtesy of @MLXXXp - BSD 3-Clause + various other Licences
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
    for (uint8_t i = 0; i < MAX_LINES; ++i) {
        lines[i].setPoints(0, 0, 0, 0);
    }
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
    if (firstLoad) {
        ball.setLaunchAngle(35);
        ball.setLaunchPower(1);
    }

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
    static uint8_t heldFrames = 0;

    // Draw the bottom UI bar and elements
    a.fillRect(0, 55, 128, 15, BLACK);
    a.drawLine(0, 55, 128, 55, WHITE);

    // Reset level at any time
    if (a.justReleased(B_BUTTON)) {
        if (heldFrames == 70) {
            DEBUG_PRINTLN(F("\nRESET triggered"));
            heldFrames = 0;
            levelState = LevelState::ResetLevel;
        }
        heldFrames = 0;
    } else if (a.pressed(B_BUTTON)) {
        a.fillRect(0, 57, heldFrames * 2, 8, WHITE);
        font3x5.setCursor(3, 57);
        font3x5.setTextColor(BLACK);
        font3x5.print(F("RESTART LEVEL..."));
        font3x5.setTextColor(WHITE);
        if (heldFrames < 70) {
            heldFrames++;
        }
    } else {
        // Draw Level Number
        uint8_t levelNumb = currentLevel + 1;
        uint8_t levelDigits = (levelNumb >= 100) ? 2 : (levelNumb >= 10) ? 1 : 0;
        uint8_t rectWidth = 5 + levelDigits * 4; // Each digit adds 4 to width
        a.fillRect(0, 56, rectWidth, 8, WHITE);
        font3x5.setTextColor(BLACK);
        font3x5.setCursor(1,57);
        font3x5.print(currentLevel+1);
        font3x5.setTextColor(WHITE);

        // Draw Angle
        font3x5.setCursor(uiAngleX, 57);
        font3x5.print(F("ANGLE:"));
        uint16_t launchAngle = playerBall.getLaunchAngle();
        font3x5.print(launchAngle);
        uint8_t rectX = uiAngleX + 7 * 4 + ((launchAngle >= 10) ? 4 : 0) + ((launchAngle >= 100) ? 4 : 0);
        a.drawRect(rectX, 58, 3, 3, WHITE);

        // Draw Power
        font3x5.setCursor(uiPowerX, 57);
        font3x5.print(F("POWER:"));
        uint8_t launchPowerIndex = playerBall.getLaunchPowerIndex();
        for (uint8_t i = 0; i < launchPowerIndex; i++) {
            a.fillRect((uiPowerX + 25 + i * 4), (62 - i), 3, i + 1);
        }
    }
}

void drawLevelUIBanner() {
    a.fillRect(0, 9, 128, 12, BLACK);
    a.drawLine(0, 10, 128, 10, WHITE);
    a.drawLine(0, 19, 128, 19, WHITE);
}

// LevelState::Reset
void updateResetLevel() {
    firstLoad = false;
    levelState = LevelState::Load;
}

// LevelState::Aim
void updateAim() {
    // Update launch power
    uint8_t launchPowerIndex = playerBall.getLaunchPowerIndex();
    if (a.justPressed(UP_BUTTON)) {
        playerBall.setLaunchPower(min(launchPowerIndex + 1, 5));
    } else if (a.justPressed(DOWN_BUTTON)) {
        playerBall.setLaunchPower(max(launchPowerIndex - 1, 1));
    }

    // Set launchAngle (Left/Right)
    uint16_t launchAngle = playerBall.getLaunchAngle();
    #define HELD_FRAMES_DELAY 45
    #define HELD_FRAMES_FREQ 3
    static uint16_t heldFramesCount = 0;

    if (a.justPressed(RIGHT_BUTTON)) {
        playerBall.setLaunchAngle((launchAngle + 1) % 360);
    } else if (a.justPressed(LEFT_BUTTON)) {
        playerBall.setLaunchAngle((launchAngle + 360 - 1) % 360);
    } else if (a.pressed(RIGHT_BUTTON)) {
        if (heldFramesCount > HELD_FRAMES_DELAY && heldFramesCount % HELD_FRAMES_FREQ == 0) {
            playerBall.setLaunchAngle((launchAngle + 1) % 360);
        }
        heldFramesCount++;
    } else if (a.pressed(LEFT_BUTTON)) {
        if (heldFramesCount > HELD_FRAMES_DELAY && heldFramesCount % HELD_FRAMES_FREQ == 0) {
            playerBall.setLaunchAngle((launchAngle + 360 - 1) % 360);
        }
        heldFramesCount++;
    } else {
        heldFramesCount = 0;  // Reset the held frames count if no button is pressed
    }

    if (a.justPressed(A_BUTTON)) {
        DEBUG_PRINTLN("LAUNCHED!\nEXIT LS::AIM");
        playerBall.launch();
        levelState = LevelState::Launch;
    }
}

void drawAim() {
    drawLevel();
    playerBall.drawAim();
    drawLevelUI();
}

// LevelState::Launch
void updateLaunch() {
    // Move ball
    playerBall.update();

    // Debug: Print ball position
    DEBUG_PRINT("\nBall position: x = ");
    DEBUG_PRINT(playerBall.getX());
    DEBUG_PRINT(", y = ");
    DEBUG_PRINTLN(playerBall.getY());


    // Ball Offscren Check + Timer
    static uint8_t offscreenTimer = 0;
    #define OFFSCREEN_TIMELIMIT 120
    if (playerBall.offscreenCheck()) {
        offscreenTimer++;
        if (offscreenTimer > OFFSCREEN_TIMELIMIT) {
            levelState = LevelState::LevelLose;
            return;  // Exit early since we've changed the state
        }
    } else {
        offscreenTimer = 0;  // Reset the timer if the ball is onscreen
    }

    // Collide Ball-Line
    for (const Line& line : levelLines) {
        // Debug: Print line being checked
        DEBUG_PRINT("Checking line: x1=");
        DEBUG_PRINT(line.getStartPos().x);

        if (playerBall.collideLineCheck(line.getStartPos(), line.getEndPos())) {
            // If colliding, bounce the ball
            // Debug: Print collision information
            DEBUG_PRINTLN(" Collision detected, ball bounced");
            playerBall.collideLineBounce(line.getStartPos(), line.getEndPos());
        } else {
            DEBUG_PRINTLN(" No Collsiion");
        }
    }
    // Collide Ball-Goal
    if (playerBall.collideGoalCheck(levelGoal.getPos(), levelGoal.getRadius())) {
        levelState = LevelState::LevelWin;
    }
}

void drawLaunch() {
    drawLevel();
    drawLevelUI();
}

// LevelState::Win
void updateWinLevel() {
    if (a.justPressed(A_BUTTON)) {
        currentLevel++;
        firstLoad = true;
        levelState = LevelState::Load;
    }
}
void drawWinLevel() {
    drawLevel();
    drawLevelUI();
    drawLevelUIBanner();
    font3x5.setCursor((64 - 5*3), 12);
    font3x5.print("GOAL!");
}

// LevelState::Lose
void updateLoseLevel() {

}
void drawLoseLevel() {
    drawLevel();
    drawLevelUI();
    drawLevelUIBanner();
    font3x5.setCursor((64 - 12*3), 12);
    font3x5.print("OUT OF BOUNDS!");
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