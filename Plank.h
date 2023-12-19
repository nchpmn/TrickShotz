#ifndef PLANK_H
#define PLANK_H

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

private:
    float vectorSquared(const Vector& vector) {
        // Calculate the 
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
};

#endif