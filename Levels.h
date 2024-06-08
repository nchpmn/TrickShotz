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
#define MAX_LINES 10

struct LevelData {
    BallData ball;
    GoalData goal;
    uint8_t numLines; // Number of lines in the level
    LineData lines[MAX_LINES];
};



// INDIVIDUAL LEVELS
const LevelData level01 PROGMEM = {
    { 10, 10, 3 },  // Ball: x, y, radius
    { 64, 32, 6 },  // Goal: x, y, radius
    3,  // Number of lines
    {
        { 20, 20, 50, 60 },  // Lines: x1, y1, x2, y2
        { 110, 10, 110, 50 },
        { 30, 15, 95, 15 },
    }
};

const LevelData level02 PROGMEM = {
    { 30, 30, 5 },  // Ball: x, y, radius
    { 100, 50, 10 },  // Goal: x, y, radius
    2,  // Number of lines
    {
        { 10, 10, 20, 20 },  // Lines: x1, y1, x2, y2
        { 40, 40, 50, 50 },
    }
};

const LevelData level03 PROGMEM = {
    { 20, 20, 4 },  // Ball: x, y, radius
    { 80, 40, 8 },  // Goal: x, y, radius
    3,  // Number of lines
    {
        { 15, 15, 25, 25 },  // Lines: x1, y1, x2, y2
        { 35, 35, 45, 45 },
        { 55, 55, 65, 65 },
    }
};

const LevelData levels[] PROGMEM = { level01, level02, level03 };
const uint8_t NUM_LEVELS = sizeof(levels) / sizeof(LevelData);



// FUNCTIONS
void loadLevelData(const LevelData *level, Ball &ball, Goal &goal, Line *lines, uint8_t &numLines) {
    // Load ball data
    ball.setPosition(level->ball.x, level->ball.y);
    ball.setRadius(level->ball.radius);

    // Load goal data
    goal.setPosition(level->goal.x, level->goal.y);
    goal.setRadius(level->goal.radius);

    // Load line data
    numLines = level->numLines;
    for (uint8_t i = 0; i < numLines; ++i) {
        lines[i] = Line(level->lines[i].x1, level->lines[i].y1, level->lines[i].x2, level->lines[i].y2);
    }
}


#endif // LEVELS_H