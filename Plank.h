#ifndef PLANK_H
#define PLANK_H

#include <Arduboy2.h>

class Plank {
    public:
        // Coordinates of one end
        int16_t x1 = 0;
        int16_t y1 = 0;

        // Coordinates of the other end
        int16_t x2 = 0;
        int16_t y2 = 0;

        // Thickness of the plank
        uint8_t thickness = 2;

        // The components of the normal vector
        float normalX;
        float normalY;

        // Default constructor with default values
        Plank() = default;

        Plank(int16_t startX1, int16_t startY1, int16_t startX2, int16_t startY2) :
            x1(startX1), y1(startY1), x2(startX2), y2(startY2) {
            // Pre-calculate the normal vector components
            getNormals();
        }

        bool checkCollision(int16_t ballX, int16_t ballY, uint8_t ballRadius) {
            // Calculate the distance between the ball's center and the line segment
            float distance = distanceToLine(x1, y1, x2, y2, ballX, ballY);

            // Check if the distance is less than the sum of the ball's radius and the Plank's thickness
            return distance <= ballRadius + (thickness / 2);
        }

        void draw() const {
            if (x1 == x2) {
                // Case where line is vertical
                a.fillRect(x1, y1, thickness, y2-y1, WHITE);
            } else if (y1 == y2) {
                // Case where line is horizontal
                a.fillRect(x1, y1, x2-x1, thickness, WHITE);
            } else {
                // Case where line is diagonal
                if (thickness % 2 == 0) {
                    uint8_t halfThick = thickness / 2;
                    a.fillTriangle(x1, y1-halfThick, x2, y2-halfThick, x1, y1+halfThick, WHITE);
                    a.fillTriangle(x2, y2-halfThick, x2, y2+halfThick, x1, y1+halfThick, WHITE);
                } else {
                    a.fillTriangle(x1, y1, x2, y2, x1, y1+thickness, WHITE);
                    a.fillTriangle(x2, y2, x2, y2+thickness, x1, y1+thickness, WHITE);
                }
            }
        }
    
    private:
        void getNormals() {
            // Calculate and store the normal vector components on creation
            float dx = x2 - x1;
            float dy = y2 - y1;
            float length = sqrt(dx*dx + dy*dy);

            normalX = dy / length;
            normalY = -dx / length; // Negative sign for the Y component since we want perpendicular vector
            
            // Ensure that the normal vector points outward
            // This should allow collisions on both sides of line
            if (normalX < 0) {
                normalX = -normalX;
                normalY = -normalY;
            }
        }

        float distanceToLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t ballX, int16_t ballY) {
            // Vector from (x1,y1) to (x2,y2)
            float segmentVectorX = x2 - x1;
            float segmentVectorY = y2 - y1;

            // Vector from (x1,y1) to (ballX,ballY)
            float pointVectorX = ballX - x1;
            float pointVectorY = ballY - y1;

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
            float distance = sqrt((ballX - closestX) * (ballX - closestX) + (ballY - closestY) * (ballY - closestY));

            return distance;

        }
};


#endif