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
        float cx = x;
        float cy = y;

        // Vector from point A to B
        float ABx = x2 - x1;
        float ABy = y2 - y1;
        
        // Vector from point A to the circle center
        float ACx = cx - x1;
        float ACy = cy - y1;

        // Project vector AC onto AB to get the closest point on the line segment
        float AB_mag_sq = ABx * ABx + ABy * ABy; // Magnitude squared of AB
        float dot_product = ACx * ABx + ACy * ABy;
        float t = dot_product / AB_mag_sq;

        // Clamp t to the range [0, 1] to ensure the closest point is on the line segment
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;

        // Find the closest point on the line segment
        float closestX = x1 + t * ABx;
        float closestY = y1 + t * ABy;

        // Compute the distance from the circle center to the closest point
        float distanceX = cx - closestX;
        float distanceY = cy - closestY;
        float distance_sq = distanceX * distanceX + distanceY * distanceY;

        // Check if the distance is less than or equal to the radius squared
        return distance_sq <= radius * radius;
    }


private:
    uint8_t x;
    uint8_t y;
    uint8_t radius;

};

#endif // BALL_H