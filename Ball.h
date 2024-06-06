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

    bool collideLine(int x1, int y1, int x2, int y2, int thickness) const {
        // Calculate the distance between the ball's center and the line segment
        // Vector from (x1,y1) to (x2,y2)
        float segmentVectorX = x2 - x1;
        float segmentVectorY = y2 - y1;

        // Vector from (x1,y1) to (ballX,ballY)
        float pointVectorX = x - x1;
        float pointVectorY = y - y1;

        // Dot Product of these two Vectors
        float dotProduct = (pointVectorX * segmentVectorX) + (pointVectorY * segmentVectorY);

        // Length of the segment vector squared
        float segmentLengthSquared = (segmentVectorX * segmentVectorX) + (segmentVectorY * segmentVectorY);

        // Calculate the parameter t (the position on the line segment) at which the closest point on the line occurs
        float t = dotProduct / segmentLengthSquared;

        // Clamp t to bounds of line segment
        t = max(0.0f, min(1.0f, t));

        // Coords of closest point on the line
        float closestX = x1 + t * segmentVectorX;
        float closestY = y1 + t * segmentVectorY;

        // Distance between (BallX,BallY) and (closestX,closestY)
        float distance = sqrt((x - closestX) * (x - closestX) + (y - closestY) * (y - closestY));

        // Check if the distance is less than the sum of the ball's radius and the Plank's thickness
        return distance < radius + (thickness / 2);
    }


private:
    uint8_t x;
    uint8_t y;
    uint8_t radius;

};

#endif // BALL_H