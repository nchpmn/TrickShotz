// TrickShotz
// A Physics-Based Puzzler
// Copyright Nathan Chapman / nchpmn
//
// 01 Nov 2023 - Development Started
// PROTOTYPE ONLY

// LIBRARIES
#include <Arduboy2.h>
Arduboy2 a;
// Micro font courtesy of @filmote - BSD 3-Clause License
#include "src/Font3x5.h"
Font3x5 font3x5 = Font3x5();


// CONSTANTS
const uint8_t FRAME_RATE = 60;
const uint8_t MAX_LEVELS = 2;
const float GRAVITY = 0.05;
const uint8_t MAX_PLANKS = 6;
const float BOUNCE_FRICTION = 0.97;
const uint8_t OFFSCREEN_SECONDS = 1;


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
    Aim,
    Play,
    LevelWin,
    LevelLose
};
LevelState levelState = LevelState::Load;


// FORWARD DELCARATIONS
class Plank;
class Ball;
class Goal;
void levelLose();

// CLASSES
class Plank {
    public:
        int16_t x1, y1; // Coordinates of one end
        int16_t x2, y2; // Coordinates of the other end
        uint8_t thickness; // Thickness of the plank
        float normalX; // X component of the normal vector
        float normalY; // Y component of the normal vector

        // Default constructor with default values
        Plank() : x1(0), y1(0), x2(0), y2(0), thickness(1) {}

        Plank(int16_t startX1, int16_t startY1, int16_t startX2, int16_t startY2) {
            x1 = startX1;
            y1 = startY1;
            x2 = startX2;
            y2 = startY2;
            thickness = 2;

            // Pre-calculate the normal vector components
            getNormals();
        }

        bool checkCollision(int16_t ballX, int16_t ballY, uint8_t ballRadius) {
            // Calculate the distance between the ball's center and the line segment
            float distance = distanceToLine(x1, y1, x2, y2, ballX, ballY);

            // Check if the distance is less than the sum of the ball's radius and the Plank's thickness
            return distance <= ballRadius + (thickness / 2);
        }

        void draw() const {
            if (x1 == x2) {
                // Case where line is vertical
                a.fillRect(x1, y1, thickness, y2-y1, WHITE);
            } else if (y1 == y2) {
                // Case where line is horizontal
                a.fillRect(x1, y1, x2-x1, thickness, WHITE);
            } else {
                // Case where line is diagonal
                if (thickness % 2 == 0) {
                    uint8_t halfThick = thickness / 2;
                    a.fillTriangle(x1, y1-halfThick, x2, y2-halfThick, x1, y1+halfThick, WHITE);
                    a.fillTriangle(x2, y2-halfThick, x2, y2+halfThick, x1, y1+halfThick, WHITE);
                } else {
                    a.fillTriangle(x1, y1, x2, y2, x1, y1+thickness, WHITE);
                    a.fillTriangle(x2, y2, x2, y2+thickness, x1, y1+thickness, WHITE);
                }
            }
        }
    
    private:
        void getNormals() {
            // Calculate and store the normal vector components on creation
            float dx = x2 - x1;
            float dy = y2 - y1;
            float length = sqrt(dx*dx + dy*dy);

            normalX = dy / length;
            normalY = -dx / length; // Negative sign for the Y component since we want perpendicular vector
            
            // Ensure that the normal vector points outward
            // This should allow collisions on both sides of line
            if (normalX < 0) {
                normalX = -normalX;
                normalY = -normalY;
        }
        }

        float distanceToLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t ballX, int16_t ballY) {
            // Vector from (x1,y1) to (x2,y2)
            float segmentVectorX = x2 - x1;
            float segmentVectorY = y2 - y1;

            // Vector from (x1,y1) to (ballX,ballY)
            float pointVectorX = ballX - x1;
            float pointVectorY = ballY - y1;

            // Dot Product of these two Vectors
            float dotProduct = (pointVectorX * segmentVectorX) + (pointVectorY * segmentVectorY);

            // Length of the segment vector squared
            float segmentLengthSquared = (segmentVectorX * segmentVectorX) + (segmentVectorY * segmentVectorY);

            // Calculate the parameter t (the position on the line segment) at which the closest point on the line occurs
            float t = dotProduct / segmentLengthSquared;

            // Clamp t to bounds of line segment
            t = max(0.0f, min(1.0f, t));

            // Coords of closest point on the line
            float closestX = x1 + t * segmentVectorX;
            float closestY = y1 + t * segmentVectorY;

            // Distance between (BallX,BallY) and (closestX,closestY)
            float distance = sqrt((ballX - closestX) * (ballX - closestX) + (ballY - closestY) * (ballY - closestY));

            return distance;

        }
};

class Ball{
    public:
        float x, y; // Position
        float vx, vy; // Velocity (X and Y components)
        uint8_t size; // Radius of ball
        uint8_t offscreenTimer; // Millisecond timer for ball offscreen
        int launchPower; // Index of launchPowerLevels[] array
        float launchAngle; // Launch angle 0.0 to 359.9

        // Default constructor with default values
        Ball() : x(0), y(0), size(1) {}


        Ball(float startX, float startY, uint8_t startSize) {
            x = startX;
            y = startY;
            vx = 0;
            vy = 0;
            size = startSize;
            launchAngle = 0.0;
            launchPower = 1;
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
            a.fillCircle(static_cast<int16_t>(round(x)), static_cast<int16_t>(round(y)), size, WHITE);
        }

        void setVelocity() {
            // Adjust launchAngle so 0 deg = up
            float adjustedAngle = launchAngle - 90;
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
            // Draw the single-pixel dot border
            int numDots = 20;  // Adjust this value for the number of dots
            for (int i = 0; i < numDots; i++) {
                float angle = TWO_PI / numDots * i;
                int x1 = x + (radius + 1) * cos(angle);
                int y1 = y + (radius + 1) * sin(angle);
                a.drawPixel(x1, y1, WHITE);
            }
        }
};

// GLOBAL VARIABLES
Ball currentBall;
Goal currentGoal;
Plank currentPlanks[MAX_PLANKS];

// LEVELS
struct LevelData { 
    Ball ball;
    Goal goal;
    Plank planks[MAX_PLANKS];
};
LevelData levels[MAX_LEVELS]; // An array of LevelData objects to store all levels
uint8_t currentLevel = 0;

void defineLevels() {
    // Level 0
    levels[0].ball = { 30, 20, 2 };
    levels[0].goal = { 100, 50, 8 };
    levels[0].planks[0] = {10, 40, 100, 58}; // Plank 0 (Diagonal)
    levels[0].planks[1] = {80, 15, 80, 25};  // Plank 1 (Vertical)
    levels[0].planks[2] = {3, 60, 67, 60};   // Plank 2 (Horizontal)

    // Level 1
    levels[1].ball = { 64, 16, 2 };
    levels[1].goal = { 64, 55, 5 } ;
    levels[1].planks[0] = {20, 15, 20, 45};
    levels[1].planks[1] = {108, 15, 108, 45};
}

void loadLevel(uint8_t n) {
    // Reset all game objects using data from levels[n]
    currentBall = levels[n].ball;
    currentGoal = levels[n].goal;
    for (uint8_t i = 0; i < MAX_PLANKS; i++) {
        currentPlanks[i] = levels[n].planks[i];
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

    // Draw Angle
    font3x5.setCursor(uiAngleX, 57);
    font3x5.print(F("ANGLE:"));
    font3x5.print(String(currentBall.launchAngle, 1));

    // Draw Power
    font3x5.setCursor(uiPowerX, 57);
    font3x5.print(F("POWER:"));
        for (int i = 0; i < currentBall.launchPower; i++) {
        a.fillRect((uiPowerX + 25 + i*4), (62 - i), 3, i+1);
    }
}

void playGame() {
    switch(levelState) {
        case LevelState::Load:
            loadLevel(currentLevel);
            levelState = LevelState::Aim;
        
        case LevelState::Aim:
            if (a.justPressed(A_BUTTON)) {
                currentBall.setVelocity();
                levelState = LevelState::Play;
            }

            // Set launchAngle (Left/Right)
            if (a.justPressed(RIGHT_BUTTON)) {
                if (currentBall.launchAngle < 359.9) {
                    currentBall.launchAngle += 10;
                } else {
                    currentBall.launchAngle = 0.0;
                }
            }
            if (a.justPressed(LEFT_BUTTON)) {
                if (currentBall.launchAngle > 0.1) {
                    currentBall.launchAngle -= 10;
                } else {
                    currentBall.launchAngle = 350;
                }
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
                levelState = LevelState::LevelWin;
            }
            currentBall.update(currentPlanks, MAX_PLANKS);
            break;
        
        case LevelState::LevelWin:
            a.print("Level Clear!");

            if (a.justPressed(A_BUTTON)) {
                currentLevel++;
                levelState = LevelState::Load;
            }
            break;
        case LevelState::LevelLose:
            a.print("You Lose!");

            if (a.justPressed(B_BUTTON)) {
                levelState = LevelState::Load;
            }
            break;
    }

    drawObjects();
    drawUI();

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

    defineLevels();
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
            a.print("Main Title\n\nA to Play\nB for Instructions");
            if (a.justPressed(A_BUTTON)) {
                gameState = GameState::PlayGame;
            }
            if (a.justPressed(B_BUTTON)) {
                gameState = GameState::Instructions;
            }
            break;
        case GameState::Instructions:
            a.print("Instructions");
            if (a.justPressed(B_BUTTON)) {
                gameState = GameState::Title;
            }
            break;
        case GameState::PlayGame:
            playGame();
            break;
        case GameState::EndScreen:
            a.print("End Screen");
            break;
    }
    
    a.display();
}