#ifndef BALL_H
#define BALL_H

#include <Arduboy2.h>

class Ball {
public:
    Pos<float> position = Pos<float>(64, 32);
    Vector velocity = Vector(0, 0);

    // Radius of ball
    uint8_t size = 2;

    // Index of launchPowerLevels[] array
    uint8_t launchPower = 2;
    // Launch angle 0 to 359
    uint16_t launchAngle = 25;

    // Default constructor with default values
    Ball() = default;

    Ball(float startX, float startY) : position(startX, startY) {
    };

    // MOVE COLLIDE FUNCTIONS HERE? - I.E.
    // 1. Move Ball
    // 2. Check collide with Planks (and update ball)
    // 3. Check collie with Goal (and upload level complete)
    // 4. Draw Ball

    void update() {
        // 1. Move Ball
        move();
        // 2. Check collision with Planks (and bounce)
        collidePlanks();
        // 3. Check collision with Goal (and update level complete)
        collideGoal();
    }

    void draw() const {
        drawBall();
        if (levelState == LevelState::Aim) {
            drawAiming();
        }
    }

private:
    // Preset power levels for launch - accessed via index of launchPower
    float launchPowerLevels[5] = {0.5, 1, 1.5, 2, 2.5};
    // Millisecond timer for ball offscreen
    uint8_t offscreenTimer = 0;

    void move() {
        // Apply gravity to ball's velocity (acceleration each frame)
        velocity.dy += GRAVITY;

        // Update ball position
        position += velocity;
    };

    void collidePlanks() {
        
    }

    void drawBall() const {
        a.fillCircle(round(position.x), round(position.y), size, WHITE);
    }

    void drawAiming() const {
        // Draw a dotted line projecting where the ball will travel
        uint8_t lineLength = 25;
        int16_t adjustedAngle = launchAngle - 90;
        if (adjustedAngle < 0) {
            adjustedAngle += 360;
        }

        Pos<float> simPosition = position;
        Vector simVelocity = {
            launchPowerLevels[launchPower - 1] * cos(radians(adjustedAngle)),
            launchPowerLevels[launchPower - 1] * sin(radians(adjustedAngle)) + GRAVITY
        };

        for (int8_t t = 0; t < lineLength; t++) {
            simPosition += simVelocity;

            if (t % 3 == 0) {
                a.drawPixel(round(simPosition.x), round(simPosition.y), WHITE);
            }
        }
    }


};


#endif