#ifndef LEVELS_H
#define LEVELS_H

// OBJECT DATA SCTRUCTURES
struct BallData {
    uint8_t x;
    uint8_t y;
    uint8_t radius;
};

struct GoalData {
    uint8_t x;
    uint8_t y;
    uint8_t radius;
};

struct LineData {
    uint8_t x1;
    uint8_t y1;
    uint8_t x2;
    uint8_t y2;
};


// LEVELS DATA STRUCUTRE
#define MAX_LINES 10

struct LevelData {
    BallData ball;
    GoalData goal;
    uint8_t numLines; // Number of lines in the level
    LineData lines[MAX_LINES];
};



#endif // LEVELS_H