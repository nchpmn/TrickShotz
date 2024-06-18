#ifndef GOAL_H
#define GOAL_H

class Goal {
public:
    Goal(int x, int y, int radius) : position(x, y), radius(radius) {}

    void draw() {
        static float rotationAngle = 0.0;
        float dashLength = (2 * PI * radius) / dashNumber;

        for (int i = 0; i < dashNumber; i += 3) {
            float angle1 = TWO_PI / dashNumber * i + rotationAngle;
            float angle2 = TWO_PI / dashNumber * (i + 1) + rotationAngle;

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

    Pos<uint8_t> getPos() const { return position; }
    int getX() const { return position.x; }
    int getY() const { return position.y; }
    int getRadius() const { return radius; }
    void setPosition(uint8_t newX, uint8_t newY) {
        position.set(newX,newY);
    }
    void setRadius(uint8_t newRadius) { radius = newRadius; }


private:
    Pos<uint8_t> position;
    uint8_t radius;
    uint8_t dashNumber = 17;

};



#endif // GOAL_H