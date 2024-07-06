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
        // Add Gravity to Velocity
        velocity.dy += gravity;
        // Calc the next frame's position
        position.x += velocity.dx;
        position.y += velocity.dy;
    }

    // Collisions: Ball-Line detection and Bounce
    bool collideLineCheck(const Pos<uint8_t>& startPoint, const Pos<uint8_t>& endPoint) {
        // Check for collision with a line
        float distance = calculateDistanceToSegment(position, startPoint, endPoint);
        return distance < radius;
    }

    void collideLineBounce(const Pos<uint8_t>& startPoint, const Pos<uint8_t>& endPoint) {
        // Calculate the closest point on the line segment to the ball's position
        Pos<float> closestPoint = calcClosestPoint(startPoint, endPoint, position);

        // Move the ball to the closest point on the line segment
        Vector normal = Vector(position - closestPoint).normalise();
        position.x = closestPoint.x + normal.dx * radius;
        position.y = closestPoint.y + normal.dy * radius;

        // Reflect the velocity vector around the normal
        float normalDotVelocity = normal.dx * velocity.dx + normal.dy * velocity.dy;
        velocity.dx -= 2 * normalDotVelocity * normal.dx;
        velocity.dy -= 2 * normalDotVelocity * normal.dy;
    }



    // Collisions: Ball-Goal detection
    bool collideGoalCheck(Pos<uint8_t> goalPos, uint8_t goalRadius) const {
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
    Pos<float> nextPosition;
    uint8_t radius;
    Vector velocity;
    uint8_t launchPowerIndex = 1; // Index of launchPowerLevels[] array
    float launchPowerLevel[5] = { 0.5, 1, 1.5, 2, 2.5 }; // Actual values used in calculations
    uint16_t launchAngle; // Launch angle 0 to 359
    float gravity = 0.05;

    // Collisions: distance between point and line
    float calculateDistanceToSegment(const Pos<float>& point, const Pos<uint8_t>& segmentStart, const Pos<uint8_t>& segmentEnd) const {
        Vector segmentVector(segmentEnd - segmentStart);
        Vector pointVector(point - segmentStart);

        float dotProduct = calcDotProduct(segmentVector, pointVector);
        float segmentLengthSquared = segmentVector.dx * segmentVector.dx + segmentVector.dy * segmentVector.dy;

        float t = dotProduct / segmentLengthSquared;
        t = clamp(t, 0.0f, 1.0f);

        Pos<float> closestPoint = calcClosestPoint(segmentStart, segmentEnd, point);
        Vector distanceVector(point - closestPoint);
        return distanceVector.magnitude();
    }

    // Utility: clamp a value within a specified range
    float clamp(float value, float minValue, float maxValue) const {
        // Ensure value is within the range [minValue, maxValue]
        return max(minValue, min(maxValue, value));
    }

    // Utility: calculate a dot product
    float calcDotProduct(const Vector& a, const Vector& b) {
        return a.dx * b.dx + a.dy * b.dy;
    }

    // Utility: cloest point on a line segment to a single point
    Pos<float> calcClosestPoint(const Pos<uint8_t>& lineStart, const Pos<uint8_t>& lineEnd, const Pos<float>& point) {
        Vector segmentVector(lineEnd - lineStart);
        Vector pointVector(point - lineStart);
        float dotProduct = calcDotProduct(segmentVector, pointVector);
        float segmentLengthSquared = segmentVector.dx * segmentVector.dx + segmentVector.dy * segmentVector.dy;
        float t = dotProduct / segmentLengthSquared;
        t = clamp(t, 0.0f, 1.0f);
        return Pos<float>(lineStart.x + t * segmentVector.dx, lineStart.y + t * segmentVector.dy);
    }
    
};

#endif // BALL_H