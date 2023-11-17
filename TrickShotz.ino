// TrickShotz
// A Physics-Based Puzzler
// Copyright Nathan Chapman / nchpmn
//
// 01 Nov 2023 - Development Started
// PROTOTYPE ONLY

// LIBRARIES
#include <Arduboy2.h>
Arduboy2 a;
#include <ArduboyTones.h>
#include <ArduboyTonesPitches.h>
ArduboyTones sound(a.audio.enabled);
// Micro font courtesy of @filmote - BSD 3-Clause License
#include "src/Font3x5.h"
Font3x5 font3x5 = Font3x5();


// CONSTANTS
const uint8_t FRAME_RATE = 60;
const uint8_t MAX_LEVELS = 5;
const float GRAVITY = 0.05;
const uint8_t MAX_PLANKS = 5;
const float BOUNCE_FRICTION = 0.9;
const uint8_t OFFSCREEN_SECONDS = 1;
const uint8_t HELD_FRAMES_DELAY = 45;
const uint8_t HELD_FRAMES_FREQ = 3;


// GRAPHICS
constexpr uint8_t title[] PROGMEM {
    128, 64,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0xC0, 0xE0, 0xE0, 0xE0, 0x60, 0x60, 0x60, 0xE0, 0xE0, 0x00, 0x00, 0x70, 0xF8, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0xC0, 0xF0, 0xF8, 0xFC, 0x3C, 0x1E, 0x0E, 0x0E, 0x0E, 0x0E, 0x1E, 0x3E, 0xFC, 0xF8, 0xF0, 0xC0, 0x0E, 0x0E, 0x0E, 0x0E, 0xFC, 0xFC, 0xFC, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x00, 0x1C, 0x1C, 0x3C, 0x38, 0x38, 0xB8, 0xF8, 0xF8, 0xF8, 0x78, 0x30, 0x10, 0xF0, 0xF0, 0xF0, 0xE0, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0x70, 0x30, 0x38, 0x38, 0x38, 0x38, 0x00, 0x3C, 0xFE, 0xFE, 0xF0, 0xE0, 0xF8, 0xFE, 0x9F, 0x8F, 0x03, 0x00, 0x00, 0x0F, 0x1F, 0x1F, 0x1C, 0x1C, 0x1C, 0x38, 0x38, 0xF8, 0xF8, 0xF0, 0xE0, 0x00, 0xFF, 0xFF, 0xFF, 0x0E, 0x0E, 0x07, 0x07, 0x07, 0x07, 0x0F, 0xFF, 0xFF, 0xF0, 0x00, 0x1F, 0x7F, 0xFF, 0xF8, 0xE0, 0xC0, 0x80, 0x80, 0x80, 0xC0, 0xE0, 0xF0, 0xFF, 0xFF, 0x3F, 0x07, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0x7E, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0xE0, 0xFF, 0xFF, 0x7F, 0x07, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0x70, 0x70, 0x70, 0xF0, 0xF0, 0xE0, 0x0E, 0x3E, 0xFF, 0xFC, 0xE0, 0x80, 0x3F, 0x7F, 0xFF, 0xF1, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0x70, 0x03, 0x1F, 0x7F, 0x3F, 0x31, 0x03, 0x07, 0x0F, 0x0F, 0x1E, 0x1C, 0x08, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x04, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F, 0x1E, 0x1E, 0x1C, 0x1C, 0x1C, 0x3C, 0x20, 0x3C, 0x7B, 0x7B, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x80, 0xC0, 0xE0, 0xE0, 0x70, 0xF8, 0xF8, 0xFC, 0x8E, 0x0E, 0x06, 0x01, 0x07, 0x1F, 0x7F, 0xFE, 0xFC, 0xFC, 0x8E, 0x0F, 0x1F, 0x1F, 0x3D, 0x38, 0x38, 0x11, 0x07, 0x0F, 0x0F, 0x0C, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0x1F, 0x1F, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x9C, 0xE4, 0xB6, 0xD6, 0xF4, 0x3C, 0x00, 0x00, 0xFC, 0x04, 0x04, 0xFC, 0xF8, 0x30, 0xFC, 0xC4, 0x84, 0xC4, 0x00, 0x00, 0xFF, 0x04, 0x04, 0xFC, 0xF8, 0x00, 0xFC, 0xCC, 0x84, 0x84, 0xFC, 0x30, 0x00, 0xFC, 0x04, 0x04, 0xFC, 0xF8, 0x04, 0x04, 0xFC, 0xF8, 0x00, 0x00, 0xFC, 0x04, 0x04, 0xFC, 0xF8, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


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
        Plank() : x1(0), y1(0), x2(0), y2(0) {
            thickness = 2;
        }

        Plank(int16_t startX1, int16_t startY1, int16_t startX2, int16_t startY2) {
            x1 = startX1;
            y1 = startY1;
            x2 = startX2;
            y2 = startY2;
            
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
    
    private:
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

// LEVELS
struct LevelData { 
    uint8_t levelBallData[2];
    uint8_t levelGoalData[3];
    uint8_t numPlanks;
    int16_t levelPlanksData[MAX_PLANKS][4];
};
const LevelData levels[MAX_LEVELS] = {
    { // Level 1
        { 30, 20 }, // Ball
        { 98, 25, 6 }, // Goal
        1, { // Planks
            { 10, 50, 118, 50 }
        }
    },
    { // Level 2
        { 30, 20 }, // Ball
        { 98, 25, 5}, // Goal
        2, { // Planks
            {50, 50, 78, 50},
            {64, 5, 64, 30},
        }
    },
    { // Level 3
        { 89, 12 }, // Ball
        { 89, 34, 5 }, // Goal
        3, { // Planks
            {69, 19, 128, 19},
            {20, 50, 128, 50},
            {20, 0, 20, 50}
        }
    },
    { // Level 4
        { 14, 42 }, // Ball
        { 106, 39, 4 }, // Goal
        5, { // Planks
            {0, 51, 128, 51},
            {0, 25, 25, 0},
            {27, 19, 69, 19},
            {67, 21, 67, 51},
            {126, 0, 126, 51}
        }
    }
    // Add more levels as needed
};
uint8_t currentLevel = 0;

void loadLevel(uint8_t n) {
    // Reset all game objects using data from levels[n]

    currentBall = Ball(levels[n].levelBallData[0], levels[n].levelBallData[1]);

    for (uint8_t i = 0; i < levels[n].numPlanks; i++) {
        currentPlanks[i].x1 = levels[n].levelPlanksData[i][0];
        currentPlanks[i].y1 = levels[n].levelPlanksData[i][1];
        currentPlanks[i].x2 = levels[n].levelPlanksData[i][2];
        currentPlanks[i].y2 = levels[n].levelPlanksData[i][3];

        currentPlanks[i].getNormals();
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
            levelState = LevelState::Load;
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
                currentLevel++;
                levelState = LevelState::Load;
            }
            break;
        case LevelState::LevelLose:
            a.drawLine(0, 10, 128, 10, WHITE);
            a.fillRect(0, 11, 128, 7, BLACK);
            font3x5.setCursor((64 - 12*3), 12);
            font3x5.print("OUT OF BOUNDS!");
            a.drawLine(0, 19, 128, 19, WHITE);
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
            font3x5.print(F("Instructions"));
            if (a.justPressed(B_BUTTON)) {
                gameState = GameState::Title;
            }
            break;
        case GameState::PlayGame:
            playGame();
            break;
        case GameState::EndScreen:
            font3x5.print(F("End Screen"));
            break;
    }
    
    a.display();
}