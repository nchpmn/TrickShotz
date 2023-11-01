// TrickShotz
// A Physics-Based Puzzler
// Copyright Nathan Chapman / nchpmn
//
// 01 Nov 2023 - Development Started

#include <Arduboy2.h>
Arduboy2 a;

const int gravity = 3;
int ballX = 10;
int ballY = 5;
int ballRadius = 3;

void setup() {
    a.begin();
    a.setFrameRate(60);
    a.display();
    a.initRandomSeed();
    a.clear();
}

void loop() {
    if (!(a.nextFrame())) {
        return;
    }
    a.pollButtons();
    a.clear();

    a.fillCircle(ballX, ballY, ballRadius, WHITE);

    ballY += gravity;
    
    a.display();
}