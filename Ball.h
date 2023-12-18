#ifndef BALL_H
#define BALL_H

#include <Arduboy2.h>

// Forward declaration
void levelLose();

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




#endif