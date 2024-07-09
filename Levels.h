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


// MACROS FOR LEVEL DATA
#define BALL_DATA(x, y, radius) { x, y, radius }
#define GOAL_DATA(x, y, radius) { x, y, radius }
#define LINE_DATA(x1, y1, x2, y2) { x1, y1, x2, y2 }
#define LEVEL_DATA(ball, goal, numLines, ...) { ball, goal, numLines, { __VA_ARGS__ } }

// INDIVIDUAL LEVELS
const LevelData levels[] PROGMEM = {
    // 01
    LEVEL_DATA(
        BALL_DATA(30, 20, 3),
        GOAL_DATA(98, 25, 10),
        1,
        LINE_DATA(10, 50, 118, 50)),
    // 02
    LEVEL_DATA(
        BALL_DATA(30, 20, 3),
        GOAL_DATA(98, 25, 5),
        2,
        LINE_DATA(50, 50, 78, 50),
        LINE_DATA(64, 5, 64, 30)),
    // 03
    LEVEL_DATA(
        BALL_DATA(89, 12, 3),
        GOAL_DATA(89, 34, 5),
        3,
        LINE_DATA(69, 19, 128, 19),
        LINE_DATA(20, 50, 128, 50),
        LINE_DATA(20, 0, 20, 50)),
    // 04
    LEVEL_DATA(
        BALL_DATA(14, 42, 3),
        GOAL_DATA(106, 39, 4),
        5,
        LINE_DATA(0, 51, 128, 51),
        LINE_DATA(0, 25, 25, 0),
        LINE_DATA(27, 19, 69, 19),
        LINE_DATA(67, 21, 67, 51),
        LINE_DATA(126, 0, 126, 51)),
    // 05
    LEVEL_DATA(
        BALL_DATA(64, 6, 3),
        GOAL_DATA(64, 50, 5),
        3,
        LINE_DATA(50, 10, 78, 10),
        LINE_DATA(0, 24, 40, 64),
        LINE_DATA(128, 24, 88, 64))
};


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