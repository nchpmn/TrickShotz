#ifndef BALL_H
#define BALL_H

class Ball {
public:
    Ball(float x, float y, int radius) : position(x, y), radius(radius) {}

    void draw() {
        a.fillCircle(position.x, position.y, radius);
    }

    void drawAim() {

    }

    void launch() {
        // Caluclate velocity based on launch settings
        velocity.dx = getLaunchPower() * cos(launchAngle * DEG_TO_RAD - PI / 2.0);
        velocity.dy = getLaunchPower() * sin(launchAngle * DEG_TO_RAD - PI / 2.0);
    }

    void update() {
        velocity.dy += gravity;

        // Calc the next frame's position
        Pos<float> nextPos;
        nextPos.x = position.x += velocity.dx;
        nextPos.y = position.y += velocity.dy;

        // Check for collision with each Line
        for (uint8_t i = 0; i < numLines; ++i) {
            if(collideLine(levelLines[i].getStartPos(), levelLines[i].getEndPos())) {
                // It collided - Bounce!

            }
        }





        position.x += velocity.dx;
        position.y += velocity.dy;
    }

    // Collisions: Ball-Line detection and Bounce
    bool collideLine(const Pos<uint8_t>& startPos, const Pos<uint8_t>& endPos) {
        // Calculate the distance from the ball's center to the line segment
        float distance = calculateDistanceToSegment(position, startPos, endPos);
        // If the distance is less than the ball's radius, a collision has occurred
        if (distance < radius) {
            // Handle the bounce effect by adjusting the ball's velocity
            bounce(startPos, endPos);
        }
    }


    // Collisions: Ball-Goal detection
    bool collideGoal(Pos<uint8_t> goalPos, uint8_t goalRadius) const {
        // 1. Define a Vector for the difference in positions
        Vector delta(position - goalPos);
        // 2. Calculate the distance using the magnitude of the Vector
        float distance = delta.magnitude();
        // 3. Check if the ball is wholly inside the goal
        return (distance + radius <= goalRadius);
    }


    Pos<float> getPos() const { return position; };
    float getX() const { return position.x; }
    float getY() const { return position.y; }
    int getRadius() const { return radius; }
    void setPosition(uint8_t newX, uint8_t newY) { position.set(newX, newY); }
    void setRadius(uint8_t newRadius) { radius = newRadius; }

    uint8_t getLaunchPowerIndex() const { return launchPowerIndex; }
    float getLaunchPower() const { return launchPowerLevel[launchPowerIndex - 1]; }
    int getLaunchAngle() const { return launchAngle; }
    void setLaunchPower(int newPower) { launchPowerIndex = newPower; }
    void setLaunchAngle(int newAngle) { launchAngle = newAngle; }


private:
    Pos<float> position;
    uint8_t radius;
    Vector velocity;
    uint8_t launchPowerIndex = 1; // Index of launchPowerLevels[] array
    float launchPowerLevel[5] = { 0.5, 1, 1.5, 2, 2.5 }; // Actual values used in calculations
    uint16_t launchAngle; // Launch angle 0 to 359
    float gravity = 0.05;

    // Collisions: distance between point and line
    float calculateDistanceToSegment(const Pos<float>& point, const Pos<uint8_t>& segmentStart, const Pos<uint8_t>& segmentEnd) const {
        // Create vectors for the segment and the point relative to the segment's start
        Vector segmentVector(segmentEnd - segmentStart);
        Vector pointVector(point - segmentStart);

        // Calculate the squared length of the segment vector
        float segmentLengthSquared = segmentVector.magnitudeSquared();
        // Compute the dot product of the point vector and the segment vector
        float dotProduct = pointVector.dot(segmentVector);
        // Calculate the parameter t that represents the closest point on the segment
        float t = dotProduct / segmentLengthSquared;
        // Clamp t to ensure it lies within the segment [0, 1]
        t = clamp(t, 0.0f, 1.0f);

        // Determine the closest point on the segment using parameter t
        Pos<float> closestPoint(segmentStart.x + t * segmentVector.dx, segmentStart.y + t * segmentVector.dy);
        // Create a vector from the closest point to the given point
        Vector distanceVector(point - closestPoint);
        // Return the magnitude of the distance vector, which is the shortest distance from the point to the segment
        return distanceVector.magnitude();
    }

    // Function to handle bouncing off a line
    void bounce(const Pos<uint8_t>& startPos, const Pos<uint8_t>& endPos) {
        // Create a vector for the segment
        Vector segmentVector(endPos - startPos);
        // Calculate the normal vector to the segment
        Vector normal(-segmentVector.dy, segmentVector.dx);
        // Normalize the normal vector
        normal.normalize();

        // Compute the dot product of the velocity and the normal vector
        float dotProduct = velocity.dot(normal);
        // Reflect the velocity vector across the normal vector to simulate a bounce
        velocity.dx -= 2 * dotProduct * normal.dx;
        velocity.dy -= 2 * dotProduct * normal.dy;
    }

    // Utility function to clamp a value within a specified range
    float clamp(float value, float minValue, float maxValue) const {
        // Ensure value is within the range [minValue, maxValue]
        return max(minValue, min(maxValue, value));
    }
    
};

#endif // BALL_H