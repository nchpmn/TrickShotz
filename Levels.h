#ifndef LEVELS_H
#define LEVELS_H

// CONSTANTS
const uint8_t MAX_LEVELS = 5;
const uint8_t MAX_PLANKS = 5;


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
        { 98, 25, 10 }, // Goal
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
    },
    { // Level 5
        { 64, 6 }, // Ball
        { 64, 50, 5 }, // Goal
        3, { // Planks
            {50, 10, 78, 10},
            {0, 24, 40, 64},
            {128, 24, 88, 64}
        }
    }
    // Add more levels as needed
};

#endif