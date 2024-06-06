#ifndef BALL_H
#define BALL_H

class Ball {
public:
    Ball(int x, int y, int radius) : x(x), y(y), radius(radius) {}

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    void draw() {
        a.fillCircle(x, y, radius);
    }

    bool collideLine(int x1, int y1, int x2, int y2) {
        // Placeholder for now
        return false;
    }


private:
    uint8_t x;
    uint8_t y;
    uint8_t radius;

};

#endif // BALL_H