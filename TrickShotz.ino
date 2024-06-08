// TrickShotz
// A Physics-Based Puzzler
// Copyright Nathan Chapman / nchpmn
// Licenced under GNU GPL 3.0
// Other Code & Libraries used as noted
//
// 01 Nov 2023 - Development Started
// 04 December 2023 - First Demo Released
// 06 June 2024 - Prototype-Collide Branch Started
// 07 June 2024 - Prototype-LoadLevels Branch Started
// 
// PROTOTYPE GOALS:
//   1. Add Goal class & win conditions
//   2. Create level loading data structure & methods

#define VERSION "v240607 PROTO-LOADLEVELS"


// LIBRARIES
#include <Arduboy2.h>
Arduboy2 a;
// Micro font courtesy of @filmote - BSD 3-Clause License
#include "src/Font3x5.h"
Font3x5 font3x5 = Font3x5();


// HEADER FILES
#include "Ball.h";
#include "Line.h";
#include "Goal.h";


// GLOBAL DEFINITIONS
#define FRAME_RATE 60
#define MOVE_SPEED 1


// CLASS OBJECTS
Ball playerBall(10, 10, 3);
Line lines[] = {
    Line(20, 20, 50, 50),
    Line(120, 50, 35, 64),
    Line(110, 10, 110, 50),
    Line(30, 15, 95, 15)
};
Goal levelGoal(64, 32, 6);


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

    // Toggle showing version number
    static bool showVersion = false;
    if (showVersion) {
        font3x5.setCursor(0,50);
        font3x5.print(VERSION);
    }
    if (a.justPressed(B_BUTTON)) {
        showVersion = !showVersion;
    }

    // Title
    font3x5.setCursor(0,0);
    font3x5.print(F("TRICKSHOTZ PROTOTYPE"));

    // Move and draw playerBall
    static bool moveFast = true;
    if (a.justPressed(A_BUTTON)) {
        moveFast = !moveFast;
    }
    if (moveFast) {
        if (a.pressed(LEFT_BUTTON)) { playerBall.move(MOVE_SPEED * -1, 0); };
        if (a.pressed(RIGHT_BUTTON)) { playerBall.move(MOVE_SPEED, 0); };
        if (a.pressed(UP_BUTTON)) { playerBall.move(0, MOVE_SPEED * -1); };
        if (a.pressed(DOWN_BUTTON)) { playerBall.move(0, MOVE_SPEED); };
    } else {
        if (a.justPressed(LEFT_BUTTON)) { playerBall.move(MOVE_SPEED * -1, 0); };
        if (a.justPressed(RIGHT_BUTTON)) { playerBall.move(MOVE_SPEED, 0); };
        if (a.justPressed(UP_BUTTON)) { playerBall.move(0, MOVE_SPEED * -1); };
        if (a.justPressed(DOWN_BUTTON)) { playerBall.move(0, MOVE_SPEED); };

    }
    playerBall.draw();

    // Draw lines and detect collision
    for (const auto &l : lines) {
        l.draw();
        if (playerBall.collideLine(l.getX1(), l.getY1(), l.getX2(), l.getY2(), l.getThick())) {
            font3x5.setCursor(0,25);
            font3x5.print("Collide!");
        }
    }

    // Draw Goal
    levelGoal.draw();
    if (playerBall.collideGoal(levelGoal.getX(), levelGoal.getY(), levelGoal.getRadius())) {
        font3x5.setCursor(70,25);
        font3x5.print("GOAL!");
    }

    a.display();
}