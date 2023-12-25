// TrickShotz
// A Physics-Based Puzzler
// Copyright Nathan Chapman / nchpmn
// Licenced under GNU GPL 3.0
//
// 01 Nov 2023 - Development Started
// 04 December 2023 - First Demo Released
// 18 Dec 2023 - Major Refactor Begun
// PROTOTYPE ONLY

#define VERSION "v231223 DEV"

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
#include "Globals.h"
#include "Vector.h"
#include "Pos.h"
#include "Graphics.h"
#include "Tunes.h"
#include "Levels.h"
#include "Ball.h"

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

    a.display();
}