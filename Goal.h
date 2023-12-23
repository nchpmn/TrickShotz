#ifndef GOAL_H
#define GOAL_H

#include <Arduboy2.h>

class Goal {
public:
    Pos<uint8_t> position = Pos<uint8_t>(64, 32);
    uint8_t radius = 2;

    // Default constructor with default values
    Goal() = default;

    Goal(uint8_t x, uint8_t y, uint8_t startRadius) :
        position(x,y), radius(startRadius) {
    }

    void draw() const {
        static float rotationAngle = 0.0;
        int numDashes = 30;  // Adjust this value for the number of dashes
        float dashLength = (2 * PI * radius) / numDashes;

        for (int i = 0; i < numDashes; i += 3) {
            float angle1 = TWO_PI / numDashes * i + rotationAngle;
            float angle2 = TWO_PI / numDashes * (i + 1) + rotationAngle;

            int x1 = position.x + radius * cos(angle1);
            int y1 = position.y + radius * sin(angle1);
            int x2 = position.x + radius * cos(angle2);
            int y2 = position.y + radius * sin(angle2);

            // Draw each dash as a line segment
            a.drawLine(x1, y1, x2, y2, WHITE);
        }

        // Update rotation angle for the next frame
        rotationAngle += 0.0043625;  // 0.5 degree in radians
        if (rotationAngle >= TWO_PI) {
            rotationAngle -= TWO_PI;
        }
    }

};

#endif