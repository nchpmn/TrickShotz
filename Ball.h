#ifndef BALL_H
#define BALL_H

class Ball {
public:
    Ball(int x, int y, int radius) : position(x, y), radius(radius) {}

    void move(const Vector& velocity) {
        position.x += velocity.dx;
        position.y += velocity.dy;
    }

    void draw() {
        a.fillCircle(position.x, position.y, radius);
    }

    bool collideLine(int x1, int y1, int x2, int y2, int thickness) const {
        // NOTE: there is a known bug in this algorithm where the "underside" of horizontal and vertical lines only registers a collision 1px later than it should.
        // 1. Calculate the distance between the ball's center and the line segment
        float distance = calculateDistanceToSegment(position.x, position.y, x1, y1, x2, y2);

        // 2. Check if the distance is less than the sum of the ball's radius and the Line's thickness
        return distance < radius + (thickness / 2);
    }

    bool collideGoal(int goalX, int goalY, int goalRadius) const {
        // Calculate the distance between the centers of the ball and the goal
        int deltaX = position.x - goalX;
        int deltaY = position.y - goalY;
        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

        // Check if the ball is wholly inside the goal
        return (distance + radius <= goalRadius);
    }

    int getX() const { return position.x; }
    int getY() const { return position.y; }
    int getRadius() const { return radius; }
    void setPosition(uint8_t newX, uint8_t newY) { position.set(newX, newY); }
    void setRadius(uint8_t newRadius) { radius = newRadius; }



private:
    Pos position;
    uint8_t radius;

    // Collision Detection: method to calculate the distance between a point and a line segment
    float calculateDistanceToSegment(float pointX, float pointY, float segmentX1, float segmentY1, float segmentX2, float segmentY2) const {
        // Honestly I don't understand the mathematics in this algorithm
        // So really these comments are just for future maintenance
        // Vector from (x1,y1) to (x2,y2)
        float segmentVectorX = segmentX2 - segmentX1;
        float segmentVectorY = segmentY2 - segmentY1;
        // Vector from (x1,y1) to (pointX,pointY)
        float pointVectorX = pointX - segmentX1;
        float pointVectorY = pointY - segmentY1;
        // Dot Product of these two Vectors
        float dotProduct = (pointVectorX * segmentVectorX) + (pointVectorY * segmentVectorY);
        // Length of the segment vector squared
        float segmentLengthSquared = (segmentVectorX * segmentVectorX) + (segmentVectorY * segmentVectorY);
        // Calculate the parameter t (the position on the line segment) at which the closest point on the line occurs
        float t = dotProduct / segmentLengthSquared;
        // Clamp t to bounds of line segment
        t = clamp(t, 0.0f, 1.0f);
        // Coords of closest point on the line
        float closestX = segmentX1 + t * segmentVectorX;
        float closestY = segmentY1 + t * segmentVectorY;
        // Return the distance between (pointX, pointY) and (closestX, closestY)
        return sqrt((pointX - closestX) * (pointX - closestX) + (pointY - closestY) * (pointY - closestY));
    }

    // Collision Detection: Method to ensure a value is within a specified range
    float clamp(float value, float minValue, float maxValue) const {
        return max(minValue, min(maxValue, value));
    }

};

#endif // BALL_H