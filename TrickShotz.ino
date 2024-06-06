// TrickShotz
// A Physics-Based Puzzler
// Copyright Nathan Chapman / nchpmn
// Licenced under GNU GPL 3.0
// Other Code & Libraries used as noted
//
// 01 Nov 2023 - Development Started
// 04 December 2023 - First Demo Released
// 06 June 2024 - Prototyping Branch Started
// 
// PROTOTYPE GOALS:
//   1. Refactor collision detection
//   2. Splitting code into multiple files

#define VERSION "v240606 PROTO-CLIDE"

// LIBRARIES
#include <Arduboy2.h>
Arduboy2 a;
// Micro font courtesy of @filmote - BSD 3-Clause License
#include "src/Font3x5.h"
Font3x5 font3x5 = Font3x5();


// MAIN SETUP
void setup() {
    a.begin();
    a.setFrameRate(60);
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
    if (a.justPressed(DOWN_BUTTON)) {
        showVersion = !showVersion;
    }

    font3x5.setCursor(0,0);
    font3x5.print(F("TRICKSHOTZ PROTOTYPE"));

    a.display();
}