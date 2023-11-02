// TrickShotz
// A Physics-Based Puzzler
// Copyright Nathan Chapman / nchpmn
//
// 01 Nov 2023 - Development Started

#include <Arduboy2.h>
Arduboy2 a;

#define GRAVITY 0.05

class Ball{
    public:
        float x, y; // Position
        float vx, vy; // Velocity (X and Y components)
        uint8_t size; // Radius of ball

        Ball(float startX, float startY, float startVX, float startVY, uint8_t startSize) {
            x = startX;
            y = startY;
            vx = startVX;
            vy = startVY;
            size = startSize;
        }

        void update() {
            // Gravity applied to ball
            vy += GRAVITY;
            
            // Update the ball's position from it's velocity values
            x += vx;
            y += vy;
        }

        void draw() {
            a.fillCircle(static_cast<int16_t>(round(x)), static_cast<int16_t>(round(y)), size, WHITE);
        }
};
Ball newBall(10,20,0.5,-0.5,2);

class Plank {
    public:
        int16_t x1, y1; // Coordinates of one end
        int16_t x2, y2; // Coordinates of the other end
        uint8_t thickness; // Thickness of the plank

        Plank(int16_t startX1, int16_t startY1, int16_t startX2, int16_t startY2, uint8_t startThickness) {
            x1 = startX1;
            y1 = startY1;
            x2 = startX2;
            y2 = startY2;
            thickness = startThickness;
        }

        bool checkCollision(int16_t ballX, int16_t ballY, uint8_t ballRadius) {
            // Check if the ball is within a certain distance from the plane
            return distanceToLine(x1, y1, x2, y2, ballX, ballY) <= ballRadius + thickness;
        }

        void draw() {
            // Calculate diff in x and y
            float dx = x2 - x1;
            float dy = y2 - y1;
            // Pythagoras to get length line between (x1,y1) and (x2,y2)
            float length = sqrt(dx * dx + dy * dy);

            // Calculate the unit vector of the line
            // I'll be honest I don't understand this maths
            float ux = dx / length;
            float uy = dy / length;

            // Calculate the perpendicular vector for thickness
            float px = -uy * thickness - 1;
            float py = ux * thickness - 1;

            // Draw the four corners of the filled rectangle
            a.fillTriangle(x1 + px, y1 + py, x2 + px, y2 + py, x2 - px, y2 - py, WHITE);
            a.fillTriangle(x1 + px, y1 + py, x2 - px, y2 - py, x1 - px, y1 - py, WHITE);
        }
    
    private:
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
Plank otherPlank(5,55,120,55,4);



void setup() {
    a.begin();
    a.setFrameRate(60);
    a.initRandomSeed();
    a.clear();
}

void loop() {
    if (!(a.nextFrame())) {
        return;
    }
    a.pollButtons();
    a.clear();

    newBall.update();
    if (otherPlank.checkCollision(newBall.x, newBall.y, newBall.size)) {
        newBall.vx = 0;
        newBall.vy = 0 - GRAVITY;
    }

    newBall.draw();

    otherPlank.draw();
    
    a.display();
}