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
        { 15, 15, 95, 45 },  // Lines: x1, y1, x2, y2
        { 35, 0, 0, 45 },
        { 55, 55, 65, 65 },
    }
};

const LevelData levels[] PROGMEM = { level01, level02, level03 };
const uint8_t NUM_LEVELS = sizeof(levels) / sizeof(LevelData);

void printLevelData(const LevelData *level) {
    // Read ball data
    BallData ball;
    memcpy_P(&ball, &(level->ball), sizeof(BallData));
    DEBUG_PRINT("Ball: x = ");
    DEBUG_PRINT(ball.x);
    DEBUG_PRINT(", y = ");
    DEBUG_PRINT(ball.y);
    DEBUG_PRINT(", radius = ");
    DEBUG_PRINTLN(ball.radius);

    // Read goal data
    GoalData goal;
    memcpy_P(&goal, &(level->goal), sizeof(GoalData));
    DEBUG_PRINT("Goal: x = ");
    DEBUG_PRINT(goal.x);
    DEBUG_PRINT(", y = ");
    DEBUG_PRINT(goal.y);
    DEBUG_PRINT(", radius = ");
    DEBUG_PRINTLN(goal.radius);

    // Read number of lines
    uint8_t numLines;
    numLines = pgm_read_byte(&(level->numLines));
    DEBUG_PRINT("Number of Lines: ");
    DEBUG_PRINTLN(numLines);

    // Read line data
    for (uint8_t i = 0; i < numLines; ++i) {
        LineData line;
        memcpy_P(&line, &(level->lines[i]), sizeof(LineData));
        DEBUG_PRINT("Line ");
        DEBUG_PRINT(i);
        DEBUG_PRINT(": (");
        DEBUG_PRINT(line.x1);
        DEBUG_PRINT(", ");
        DEBUG_PRINT(line.y1);
        DEBUG_PRINT(") -> (");
        DEBUG_PRINT(line.x2);
        DEBUG_PRINT(", ");
        DEBUG_PRINT(line.y2);
        DEBUG_PRINTLN(")");
    }
}
void printAllLevels() {
    for (uint8_t i = 0; i < NUM_LEVELS; ++i) {
        DEBUG_PRINT("Level ");
        DEBUG_PRINTLN(i);
        printLevelData(&levels[i]);
        DEBUG_PRINTLN();
    }
}



// FUNCTIONS


#endif // LEVELS_H