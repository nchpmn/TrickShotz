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
#include "Graphics.h"
#include "Tunes.h"
#include "Levels.h"
#include "Plank.h"

// CONSTANTS
const uint8_t FRAME_RATE = 60;
const float GRAVITY = 0.05;
const float BOUNCE_FRICTION = 0.9;
const uint8_t OFFSCREEN_SECONDS = 1;
const uint8_t HELD_FRAMES_DELAY = 45;
const uint8_t HELD_FRAMES_FREQ = 3;


// GAME STATES (STRUCTURES)
enum class GameState {
    Title,
    Instructions,
    PlayGame,
    EndScreen
};
GameState gameState = GameState::Title;

enum class LevelState {
    Load,
    Reload,
    Aim,
    Play,
    LevelWin,
    LevelLose
};
LevelState levelState = LevelState::Load;


// FORWARD DELCARATIONS
class Ball;
class Goal;
void levelLose();

// CLASSES
class Ball {
    public:
        float x, y; // Position
        float vx, vy; // Velocity (X and Y components)
        uint8_t size; // Radius of ball
        uint8_t offscreenTimer; // Millisecond timer for ball offscreen
        uint8_t launchPower; // Index of launchPowerLevels[] array
        uint16_t launchAngle; // Launch angle 0 to 359

        // Default constructor with default values
        Ball() : x(64), y(32) {}


        Ball(float startX, float startY) {
            x = startX;
            y = startY;
            vx = 0;
            vy = 0;
            size = 2;
            launchAngle = 25;
            launchPower = 2;
        }

        void update(Plank planks[], int numPlanks) {
            // Gravity applied to ball
            vy += GRAVITY;
            
            // Calc the next frame's position
            float nextX = x + vx;
            float nextY = y + vy;

            // Check for collision with planks
            for (int i = 0; i < MAX_PLANKS; i++) {
                if (planks[i].checkCollision(nextX, nextY, size)) {
                    // Collision detected with plank[i] - Bounce!
                    // Calculate dot product of velocity and normals
                    float dotProduct = (vx * planks[i].normalX) + (vy * planks[i].normalY);

                    // Calculate reflection direction
                    float reflectionX = 2.0 * dotProduct * planks[i].normalX;
                    float reflectionY = 2.0 * dotProduct * planks[i].normalY;

                    // Update the ball's velocity
                    vx = vx - reflectionX;
                    vy = vy - reflectionY;

                    x = nextX;
                    y = nextY;

                    // Add friction from bounce
                    vx *= BOUNCE_FRICTION;
                    vy *= BOUNCE_FRICTION;

                    sound.tone(55 + (5 * i), 60);
                }
            }

            x += vx;
            y += vy;

            // Check if ball is offscreen
            if (checkOffscreen()) {
                // Start timer
                offscreenTimer++;
                if (offscreenTimer == (OFFSCREEN_SECONDS * FRAME_RATE)) {
                    levelLose();
                }
            } else {
                // Reset timer to 0
                offscreenTimer = 0;
            }
        }

        void draw() const {
            a.fillCircle(round(x), round(y), size, WHITE);

            // Draw a dotted line while aiming
            if (levelState == LevelState::Aim) {
                uint8_t lineLength = 25;
                int16_t adjustedAngle = launchAngle - 90;
                if (adjustedAngle < 0) {
                    adjustedAngle += 360;
                }
                float angleRad = radians(adjustedAngle);

                float simX = x;
                float simY = y;
                float simVX = launchPowerLevels[launchPower - 1] * cos(radians(adjustedAngle));
                float simVY = launchPowerLevels[launchPower - 1] * sin(radians(adjustedAngle));

                for (int8_t t = 0; t < lineLength; t++) {
                    // Gravity applied to ball
                    simVY += GRAVITY;

                    simX += simVX;
                    simY += simVY;

                    if (t % 3 == 0) {
                        a.drawPixel(round(simX), round(simY), WHITE);
                    }
                }
                
            }
        }

        void setVelocity() {
            // Adjust launchAngle so 0 deg = up
            int16_t adjustedAngle = launchAngle - 90;
            if (adjustedAngle < 0) {
                adjustedAngle += 360;
            }

            // Calulcate vx and vy from angle and power
            vx = launchPowerLevels[launchPower - 1] * cos(radians(adjustedAngle));
            vy = launchPowerLevels[launchPower - 1] * sin(radians(adjustedAngle));
        }
    
    private:
        float launchPowerLevels[5] = {0.5, 1, 1.5, 2, 2.5};
        
        bool checkOffscreen() {
            return (x < 0 || x > WIDTH || y < 0 || y > HEIGHT);
        }
};

class Goal {
    public:
        uint8_t x;
        uint8_t y;
        uint8_t radius;

        // Default constructor with default values
        Goal() : x(0), y(0), radius(1) {}

        Goal(uint8_t startX, uint8_t startY, uint8_t startRadius) {
            x = startX;
            y = startY;
            radius = startRadius;
        }

        bool isBallInside(const Ball& ball) {
            float distance = sqrt((ball.x - x) * (ball.x - x) + (ball.y - y) * (ball.y - y));
            return distance + ball.size <= radius + ball.size;
        }

        void draw() const {
            static float rotationAngle = 0.0;
            int numDashes = 30;  // Adjust this value for the number of dashes
            float dashLength = (2 * PI * radius) / numDashes;

            for (int i = 0; i < numDashes; i += 3) {
                float angle1 = TWO_PI / numDashes * i + rotationAngle;
                float angle2 = TWO_PI / numDashes * (i + 1) + rotationAngle;

                int x1 = x + radius * cos(angle1);
                int y1 = y + radius * sin(angle1);
                int x2 = x + radius * cos(angle2);
                int y2 = y + radius * sin(angle2);

                // Draw each dash as a line segment
                a.drawLine(x1, y1, x2, y2, WHITE);
            }

            // Update rotation angle for the next frame
            rotationAngle += 0.0043625;  // 0.5 degree in radians
            if (rotationAngle >= TWO_PI) {
                rotationAngle -= TWO_PI;
            }
        }
};

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