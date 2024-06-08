#ifndef GOAL_H
#define GOAL_H

class Goal {
public:
    Goal(int x, int y, int radius) : x(x), y(y), radius(radius) {}

    void draw() {
        static float rotationAngle = 0.0;
        float dashLength = (2 * PI * radius) / dashNumber;

        for (int i = 0; i < dashNumber; i += 3) {
            float angle1 = TWO_PI / dashNumber * i + rotationAngle;
            float angle2 = TWO_PI / dashNumber * (i + 1) + rotationAngle;

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

    int getX() const { return x; }
    int getY() const { return y; }
    int getRadius() const { return radius; }

private:
    int x;
    int y;
    int radius;
    int dashNumber = 17;

};



#endif // GOAL_H