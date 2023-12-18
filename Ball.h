#ifndef BALL_H
#define BALL_H

#include <Arduboy2.h>

// Forward declaration
void levelLose();

class Ball {
    public:
        // Position
        //float x = 64;
        //float y = 32;
        Position position = Position(64, 32);

        // Velocity (X and Y components)
        Vector velocity = Vector(0,0);
        //float vx = 0;
        //float vy = 0;

        // Radius of ball
        uint8_t size = 2;

        // Millisecond timer for ball offscreen
        uint8_t offscreenTimer = 0;

        // Index of launchPowerLevels[] array
        uint8_t launchPower = 2;

        // Launch angle 0 to 359
        uint16_t launchAngle = 25;

        // Default constructor with default values
        Ball() = default;

        Ball(float x, float y) :
            position(x,y) {
        }

        void update(Plank planks[], int numPlanks) {
            // Gravity applied to ball
            velocity.y += GRAVITY;
            
            // Calc the next frame's position
            float nextX = position.x + velocity.x;
            float nextY = position.y + velocity.y;

            // Check for collision with planks
            for (int i = 0; i < MAX_PLANKS; i++) {
                if (planks[i].checkCollision(nextX, nextY, size)) {
                    // Collision detected with plank[i] - Bounce!
                    // Calculate dot product of velocity and normals
                    float dotProduct = (velocity.x * planks[i].normalX) + (velocity.y * planks[i].normalY);

                    // Calculate reflection direction
                    float reflectionX = 2.0 * dotProduct * planks[i].normalX;
                    float reflectionY = 2.0 * dotProduct * planks[i].normalY;

                    // Update the ball's velocity
                    velocity.x = velocity.x - reflectionX;
                    velocity.y = velocity.y - reflectionY;

                    x = nextX;
                   position.y = nextY;

                    // Add friction from bounce
                    velocity.x *= BOUNCE_FRICTION;
                    velocity.y *= BOUNCE_FRICTION;

                    sound.tone(55 + (5 * i), 60);
                }
            }

            x += velocity.x;
           position.y += velocity.y;

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
                float simY =position.y;
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
            velocity.x = launchPowerLevels[launchPower - 1] * cos(radians(adjustedAngle));
            velocity.y = launchPowerLevels[launchPower - 1] * sin(radians(adjustedAngle));
        }
    
    private:
        float launchPowerLevels[5] = {0.5, 1, 1.5, 2, 2.5};
        
        bool checkOffscreen() {
            return (x < 0 || x > WIDTH ||position.y < 0 ||position.y > HEIGHT);
        }
};




#endif