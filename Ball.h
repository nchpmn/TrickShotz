#ifndef BALL_H
#define BALL_H

class Ball {
public:
    Ball(float x, float y, int radius) : position(x, y), radius(radius) {}

    void draw() {
        a.fillCircle(position.x, position.y, radius);
    }

    void drawAim() {
        if (launchChanged) {
            // Only make calculations if values changed
            int adjustedAngle = getLaunchAngle() - 90;
            if (adjustedAngle < 0) {
                adjustedAngle += 360;
            }
            float angleRad = radians(adjustedAngle);
            aimSimPos = getPos();
            aimSimVelocity.dx = getLaunchPower() * cos(angleRad);
            aimSimVelocity.dy = getLaunchPower() * sin(angleRad);
        }

        for (int t = 0; t < aimLineLength; ++t) {
            // Apply gravity to the simulated trajectory
            aimSimVelocity.dy += gravity;
            aimSimPos.x += aimSimVelocity.dx;
            aimSimPos.y += aimSimVelocity.dy;
            if (t % 4 == 0) {
                a.drawPixel(round(aimSimPos.x), round(aimSimPos.y), WHITE);
            }
        }
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
        float distance = calcDistanceToSegment(position, startPoint, endPoint);
        
        // Debugging: Print distances and positions
        DEBUG_PRINT("\nBall position: (");
        DEBUG_PRINT(position.x);
        DEBUG_PRINT(", ");
        DEBUG_PRINT(position.y);
        DEBUG_PRINT("), Start point: (");
        DEBUG_PRINT(startPoint.x);
        DEBUG_PRINT(", ");
        DEBUG_PRINT(startPoint.y);
        DEBUG_PRINT("), End point: (");
        DEBUG_PRINT(endPoint.x);
        DEBUG_PRINT(", ");
        DEBUG_PRINT(endPoint.y);
        DEBUG_PRINT("), Distance: ");
        DEBUG_PRINTLN(distance);

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

        // Apply friction to the velocity
        velocity *= friction;

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

    bool offscreenCheck() {
        if (position.x < 0 || position.x > WIDTH || 
        position.y < 0 || position.y > HEIGHT) {
            return true;
        } else {
            return false;
        }
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
    void setLaunchPower(int newPower) { 
        launchChanged = true;
        launchPowerIndex = newPower;
    }
    void setLaunchAngle(int newAngle) { 
        launchChanged = true;
        launchAngle = newAngle;
    }


private:
    Pos<float> position;
    Pos<float> nextPosition;
    uint8_t radius;
    Vector velocity;
    uint8_t launchPowerIndex = 2; // Index of launchPowerLevels[] array
    float launchPowerLevel[5] = { 0.5, 1, 1.5, 2, 2.5 }; // Actual values used in calculations
    uint16_t launchAngle = 40; // Launch angle 0 to 359
    bool launchChanged = true;
    uint8_t aimLineLength = 25;
    Pos<float> aimSimPos;
    Vector aimSimVelocity;
    float gravity = 0.06;
    float friction = 0.95;


    // Collisions: distance between point and line
    float calcDistanceToSegment(const Pos<float>& point, const Pos<uint8_t>& segmentStart, const Pos<uint8_t>& segmentEnd) const {
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
    
    // Guard against division by zero
    if (segmentLengthSquared == 0) {
        return Pos<float>(lineStart.x, lineStart.y);
    }

    float t = dotProduct / segmentLengthSquared;
    t = clamp(t, 0.0f, 1.0f);

    // Debugging output
    DEBUG_PRINT("\nSegment vector: (");
    DEBUG_PRINT(segmentVector.dx);
    DEBUG_PRINT(", ");
    DEBUG_PRINT(segmentVector.dy);
    DEBUG_PRINTLN(")");

    DEBUG_PRINT("Point vector: (");
    DEBUG_PRINT(pointVector.dx);
    DEBUG_PRINT(", ");
    DEBUG_PRINT(pointVector.dy);
    DEBUG_PRINTLN(")");

    DEBUG_PRINT("Dot product: ");
    DEBUG_PRINTLN(dotProduct);

    DEBUG_PRINT("Segment length squared: ");
    DEBUG_PRINTLN(segmentLengthSquared);

    DEBUG_PRINT("t value: ");
    DEBUG_PRINTLN(t);

    return Pos<float>(lineStart.x + t * segmentVector.dx, lineStart.y + t * segmentVector.dy);
}

    
};

#endif // BALL_H