#pragma once

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
    Ball(float startX, float startY, Plank* planks, int& numPlanks, Goal* currentGoal) : 
        position(startX, startY), planks(planks), numPlanks(numPlanks), currentGoal(currentGoal) {}

    void update() {
        // 1. Check collision with Goal (and win level)
        collideGoal();
        // 2. Move Ball
        move();
        // 3. Check collision with Planks (and bounce)
        collidePlanks();
        // 4. Check if ball offscreen and start timer
        updateOffscreen();
    }

    void draw() {
        drawBall();
        if (levelState == LevelState::Aim) {
            drawAiming();
        }
    }

    void updateLaunch() {
        // Adjust launchAngle so 0 deg = up
        int16_t adjustedAngle = adjustLaunchAngle(launchAngle);

        // Calculate velocity vector from angle and power
        Vector launchVector = {
            launchPowerLevels[launchPower - 1] * cos(radians(adjustedAngle)),
            launchPowerLevels[launchPower - 1] * sin(radians(adjustedAngle))
        };

        // Set the velocity vector
        velocity = launchVector;
    }

private:
    // Preset power levels for launch - accessed via index of launchPower
    float launchPowerLevels[5] = {0.5, 1, 1.5, 2, 2.5};
    // Millisecond timer for ball offscreen
    uint8_t offscreenTimer = 0;

    // References to planks and numPlanks and Goal
    Plank* planks;
    int& numPlanks;
    Goal* currentGoal;

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

    void collideGoal() const {
        if (ballInsideGoal()) {
            sound.tones(winSong);
            levelState = LevelState::LevelWin;
        }
    }

    bool ballInsideGoal() const {
        // Calculate the distance between the ball's center and the goal's center
        float distance = sqrt(pow(position.x - currentGoal->position.x, 2) + pow(position.y - currentGoal->position.y, 2));

        // Check if the distance is less than the sum of the radii, indicating a collision
        return (distance < (size + currentGoal->radius));
    }

    void updateOffscreen() {
        if (checkOffscreen()) {
            // Start timer
            offscreenTimer++;
            if (offscreenTimer == (OFFSCREEN_SECONDS * FRAME_RATE)) {
                // levelLose();
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

    void drawAiming() {
        // Draw a dotted line projecting where the ball will travel
        uint8_t lineLength = 25;
        int16_t adjustedAngle = adjustLaunchAngle(launchAngle);

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

    int16_t adjustLaunchAngle(int16_t angle) {
        angle -= 90;
        if (angle < 0) {
            angle += 360;
        }
        return angle;
    }

};