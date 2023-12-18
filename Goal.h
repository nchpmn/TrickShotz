#ifndef GOAL_H
#define GOAL_H

#include <Arduboy2.h>

class Goal {
    public:
        uint8_t x = 64;
        uint8_t y = 32;
        uint8_t radius = 2;

        // Default constructor with default values
        Goal() = default;

        Goal(uint8_t startX, uint8_t startY, uint8_t startRadius) :
            x (startX), y(startY), radius(startRadius) {
            
        }

        bool isBallInside(const Ball& ball) {
            float distance = sqrt((ball.x - x) * (ball.x - x) + (ball.y - y) * (ball.y - y));
            return distance + ball.size <= radius + ball.size;
        }

        void draw() const {
            static float rotationAngle = 0.0;
            int numDashes = 30;  // Adjust this value for the number of dashes
            float dashLength = (2 * PI * radius) / numDashes;

            for (int i = 0; i < numDashes; i += 3) {
                float angle1 = TWO_PI / numDashes * i + rotationAngle;
                float angle2 = TWO_PI / numDashes * (i + 1) + rotationAngle;

                int x1 = x + radius * cos(angle1);
                int y1 = y + radius * sin(angle1);
                int x2 = x + radius * cos(angle2);
                int y2 = y + radius * sin(angle2);

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