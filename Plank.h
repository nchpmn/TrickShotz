#include <Arduboy2.h>

class Plank {
public:
    Pos<int16_t> startPos = Pos<int16_t>(0, 0);
    Pos<int16_t> endPos = Pos<int16_t>(0, 0);
    uint8_t thickness = 2;
    Vector normalVector;

    Plank() = default;

    Plank(int16_t startX, int16_t startY, int16_t endX, int16_t endY) :
        startPos(startX, startY), endPos(endX, endY) {
            // Pre-calculate on creation
            calculateNormal();
        }

    void draw() {
        // Draw a line of thickness 2 (hardcoded... :S)
        a.drawLine(startPos.x, startPos.y, endPos.x, endPos.y);
        if (startPos.x != endPos.x && startPos.y != endPos.y) {
            // If line is diagonal
            a.drawLine(startPos.x + 1, startPos.y + 1, endPos.x + 1, endPos.y + 1);
        } else {
            if (startPos.x == endPos.x) {
                // If line is horizontal
                a.drawLine(startPos.x + 1, startPos.y, endPos.x + 1, endPos.y);
            } else {
                // If line is vertical
                a.drawLine(startPos.x, startPos.y + 1, endPos.x, endPos.y + 1);
            }
        }
    }

    bool checkCollision(Pos ballPos, uint8_t ballRadius) {
        // Calculate the distance between the ball's center and the line segment
        float distance = distanceToLine(ballPos);

        // Check if the distance is less than the sum of the ball's radius and the Plank's thickness
        return distance <= ballRadius + (thickness / 2);
    }

private:
    float vectorSquared(const Vector& vector) const {
        // Calculate the sum of squared vector components
        return (vector.dx * vector.dx) + (vector.dy * vector.dy);
    }

    void calculateNormal() {
        // Find the change in x and y for the line
        int16_t dx = endPos.x - startPos.x;
        int16_t dy = endPos.y - startPos.y;

        // Set values for vector. Swap dx/dy, and negate one of them.
        // This ensures the vector is perpendicular to the line
        normalVector.dx = -dy;
        normalVector.dy = dx;

        // Normalise the vector
        // i.e. Scale the vector to have a length of 1 (a unit vector)
        float length = sqrt(vectorSquared(normalVector));
        if (length != 0) {
            normalVector.dx /= length;
            normalVector.dy /= length;
        }
    }

    float distanceToLine(const Pos<int16_t>& ballPos) const {
        // Calculates the perpendicular distance from a ballPos(x,y) to the line segment of Plank
        // Create vectors
        Vector segmentVector(endPos.x - startPos.x, endPos.y - startPos.y);
        Vector pointVector(ballPos.x - startPos.x, ballPos.y - startPos.y);

        // Dot Product of these two Vectors
        float dotProduct = (pointVector.dx * segmentVector.dx) + (pointVector.dy * segmentVector.dy);

        // Length of the segment vector squared
        float segmentLengthSquared = vectorSquared(segmentVector);

        // Calculate the parameter t (the position on the line segment) at which the closest point on the line occurs
        float t = dotProduct / segmentLengthSquared;

        // Clamp t to bounds of line segment
        t = max(0.0f, min(1.0f, t));

        // Coords of closest point on the line
        Pos<int16_t> closestPoint(startPos.x + static_cast<int16_t>(t * segmentVector.dx),
                                startPos.y + static_cast<int16_t>(t * segmentVector.dy));

        // Distance between ballPos and closestPoint
        float distance = sqrt(vectorSquared(Vector{ballPos.x - closestPoint.x, ballPos.y - closestPoint.y}));

        return distance;
    }

};