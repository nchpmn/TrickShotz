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

    void drawAim() {

    }

    bool collideLine(Pos startPos, Pos endPos, int thickness) const {
        // NOTE: there is a known bug in this algorithm where the "underside" of horizontal and vertical lines only registers a collision 1px later than it should.
        // 1. Calculate the distance between the ball's center and the line segment
        float distance = calculateDistanceToSegment(position, startPos, endPos);
        // 2. Check if the distance is less than the sum of the ball's radius and the Line's thickness
        return distance < radius + (thickness / 2);
    }

    bool collideGoal(Pos goalPos, int goalRadius) const {
        // 1. Define a Vector for the difference in positions
        Vector delta(position - goalPos);
        // 2. Calculate the distance using the magnitude of the Vector
        float distance = delta.magnitude();
        // 3. Check if the ball is wholly inside the goal
        return (distance + radius <= goalRadius);
    }


    Pos getPos() const { return position; };
    int getX() const { return position.x; }
    int getY() const { return position.y; }
    int getRadius() const { return radius; }
    void setPosition(uint8_t newX, uint8_t newY) { position.set(newX, newY); }
    void setRadius(uint8_t newRadius) { radius = newRadius; }

    uint8_t getLaunchPowerIndex() const { return launchPowerIndex; }
    float getLaunchPowerLevel() const { return launchPowerLevel[launchPowerIndex]; }
    int getLaunchAngle() const { return launchAngle; }
    void setLaunchPower(int newPower) { launchPowerIndex = newPower; }
    void setLaunchAngle(int newAngle) { launchAngle = newAngle; }


private:
    Pos position;
    uint8_t radius;
    uint8_t launchPowerIndex = 1; // Index of launchPowerLevels[] array
    float launchPowerLevel[5] = { 0.5, 1, 1.5, 2, 2.5 }; // Actual values used in calculations
    uint16_t launchAngle; // Launch angle 0 to 359

    // Collision Detection: method to calculate the distance between a point and a line segment
    float calculateDistanceToSegment(const Pos& point, const Pos& segmentStart, const Pos& segmentEnd) const {
        // Honestly I don't understand the mathematics in this algorithm
        // So really these comments are just for future maintenance
        // Create a vector from start/end of segment and segmentStart to point
        Vector segmentVector(segmentEnd - segmentStart);
        Vector pointVector(point - segmentStart);

        // Compute the dot product of the point vector and the segment vector
        float dotProduct = pointVector.dx * segmentVector.dx + pointVector.dy * segmentVector.dy;

        // Calculate the squared length of the segment vector
        float segmentLengthSquared = segmentVector.dx * segmentVector.dx + segmentVector.dy * segmentVector.dy;
        
        // t represents the closest point on the segment to the given point
        float t = dotProduct / segmentLengthSquared;

        // Clamp t to ensure it lies within the segment [0, 1]
        t = clamp(t, 0.0f, 1.0f);

        // Determine the closest point on the segment using parameter t
        Pos closestPoint(segmentStart.x + t * segmentVector.dx, segmentStart.y + t * segmentVector.dy);

        // Create a vector from the closest point to the given point
        Vector distanceVector(point - closestPoint);

        // Return the magnitude of the distance vector, which is the shortest distance from the point to the segment
        return distanceVector.magnitude();
    }

    // Collision Detection: Method to ensure a value is within a specified range
    float clamp(float value, float minValue, float maxValue) const {
        return max(minValue, min(maxValue, value));
    }

};

#endif // BALL_H