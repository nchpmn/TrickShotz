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

    // Constructor with references to planks and numPlanks
    Ball(float startX, float startY, Plank* planks, int& numPlanks) : 
        position(startX, startY), planks(planks), numPlanks(numPlanks) {}


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
        // 4. Check if ball offscreen and start timer
        updateOffscreen();
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

    // References to planks and numPlanks
    Plank* planks;
    int& numPlanks;

    void move() {
        // Apply gravity to ball's velocity (acceleration each frame)
        velocity.dy += GRAVITY;

        // Update ball position
        position += velocity;
    }

    void collidePlanks() {
        for (int i = 0; i < MAX_PLANKS; i++) {
            if (planks[i].checkCollision(position, size)) {
                // Collision detected with plank[i] - Bounce!
                // Calculate dot product of velocity and normals
                float dotProduct = velocity.dot(planks[i].normalVector);

                // Calculate reflection direction
                Vector reflectionVector = planks[i].normalVector * (2.0f * dotProduct);

                // Update the ball's velocity
                velocity -= reflectionVector;

                // Move the ball slightly away from the collision point to prevent repeated collisions
                position += velocity * 0.5;

                // Add friction from bounce
                velocity *= BOUNCE_FRICTION;

                // Play a sound or perform other bounce-related actions
                sound.tone(55 + (5 * i), 60);
            }
        }
    }

    void collideGoal() {

    }

    void updateOffscreen() {
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

    bool checkOffscreen() const {
        return (position.x < 0 || position.x > WIDTH || position.y < 0 || position.y > HEIGHT);
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