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
    t = max(0.0f, min(1.0f, t));

    // Find the closest point on the line segment
    float closestX = x1 + t * ABx;
    float closestY = y1 + t * ABy;

    // Compute the distance from the circle center to the closest point
    float distanceX = cx - closestX;
    float distanceY = cy - closestY;
    float distance_sq = distanceX * distanceX + distanceY * distanceY;

    // Check if the line is not vertical or horizontal
    if (x1 != x2 && y1 != y2) {
        // Check if the distance is less than or equal to the radius plus half of the line thickness
        return distance_sq <= (radius + (thickness / 2)) * (radius + (thickness / 2));
    } else {
        // For vertical or horizontal lines, extend the collision detection region to cover both sides of the line
        float halfThickness = thickness / 2.0f;
        // Check if the closest point is within the line segment and within the thickness
        return t >= -halfThickness && t <= 1 + halfThickness && distance_sq <= radius * radius;
    }
}


private:
    uint8_t x;
    uint8_t y;
    uint8_t radius;

};

#endif // BALL_H