// TrickShotz
// A Physics-Based Puzzler
// Copyright Nathan Chapman / nchpmn
//
// 01 Nov 2023 - Development Started
// PROTOTYPE ONLY

#include <Arduboy2.h>
Arduboy2 a;

#define GRAVITY 0.05
#define MAX_PLANKS 6
#define BOUNCE_FRICTION 0.97
#define OFFSCREEN_SECONDS 1
#define FRAME_RATE 60

enum class GameState {
    Title,
    Instructions,
    PlayGame,
    EndScreen
};
GameState gameState = GameState::Title;

enum class LevelState {
    Setup,
    Play,
    LevelWin,
    LevelLose
};
LevelState levelState = LevelState::Setup;


class Plank {
    public:
        int16_t x1, y1; // Coordinates of one end
        int16_t x2, y2; // Coordinates of the other end
        uint8_t thickness; // Thickness of the plank
        float normalX; // X component of the normal vector
        float normalY; // Y component of the normal vector

        // Default constructor with default values
        Plank() : x1(0), y1(0), x2(0), y2(0), thickness(1) {}


        Plank(int16_t startX1, int16_t startY1, int16_t startX2, int16_t startY2, uint8_t startThickness) {
            x1 = startX1;
            y1 = startY1;
            x2 = startX2;
            y2 = startY2;
            thickness = 2;

            // Pre-calculate the normal vector components
            getNormals();
        }

        bool checkCollision(int16_t ballX, int16_t ballY, uint8_t ballRadius) {
            // Calculate the distance between the ball's center and the line segment
            float distance = distanceToLine(x1, y1, x2, y2, ballX, ballY);

            // Check if the distance is less than the sum of the ball's radius and the Plank's thickness
            return distance <= ballRadius + (thickness / 2);
        }

        void draw() {
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
// Create an array to hold Plank objects - MAXIMUM of 10 per level!
Plank planks[MAX_PLANKS];

void levelLose() {
    levelState = LevelState::LevelLose;
}

class Ball{
    public:
        float x, y; // Position
        float vx, vy; // Velocity (X and Y components)
        uint8_t size; // Radius of ball
        uint8_t offscreenTimer; // Millisecond timer for ball offscreen

        Ball(float startX, float startY, float startVX, float startVY, uint8_t startSize) {
            x = startX;
            y = startY;
            vx = startVX;
            vy = startVY;
            size = startSize;
        }

        void update(Plank planks[], int numPlanks) {
            // Gravity applied to ball
            vy += GRAVITY;
            
            // Calc the next frame's position
            float nextX = x + vx;
            float nextY = y + vy;

            // Check for collision with planks
            for (int i = 0; i < MAX_PLANKS; i++) {
                if (planks[i].checkCollision(nextX, nextY, size)) {
                    // Collision detected with plank[i] - Bounce!
                    // Calculate dot product of velocity and normals
                    float dotProduct = (vx * planks[i].normalX) + (vy * planks[i].normalY);

                    // Calculate reflection direction
                    float reflectionX = 2.0 * dotProduct * planks[i].normalX;
                    float reflectionY = 2.0 * dotProduct * planks[i].normalY;

                    // Update the ball's velocity
                    vx = vx - reflectionX;
                    vy = vy - reflectionY;

                    x = nextX;
                    y = nextY;

                    // Add friction from bounce
                    vx *= BOUNCE_FRICTION;
                    vy *= BOUNCE_FRICTION;

                    /*Serial.print("Bounce - Plank ");
                    Serial.print(i);
                    Serial.print("\n");
                    Serial.print(vx);
                    Serial.print("  ");
                    Serial.print(vy);
                    Serial.print("\n\n");*/
                }
            }

            x += vx;
            y += vy;

            // Check if ball is offscreen
            if (checkOffscreen()) {
                // Start timer
                offscreenTimer++;
                if (offscreenTimer == (OFFSCREEN_SECONDS * FRAME_RATE)) {
                    levelLose();
                }
            } else {
                // Reset timer to 0
                offscreenTimer = 0;
            }
        }

        void draw() {
            a.fillCircle(static_cast<int16_t>(round(x)), static_cast<int16_t>(round(y)), size, WHITE);
        }
    
    private:
        bool checkOffscreen() {
            return (x < 0 || x > WIDTH || y < 0 || y > HEIGHT);
        }
};
Ball newBall(10,20,0.5,-2,2);

class Goal {
    public:
        uint8_t x;
        uint8_t y;
        uint8_t radius;

        Goal(uint8_t startX, uint8_t startY, uint8_t startRadius) {
            x = startX;
            y = startY;
            radius = startRadius;
        }

        bool isBallInside(const Ball& ball) {
            float distance = sqrt((ball.x - x) * (ball.x - x) + (ball.y - y) * (ball.y - y));
            Serial.print(distance + ball.size);
            Serial.print("\n");
            return distance + ball.size <= radius + ball.size;
        }

        void draw() {
            // Draw the single-pixel dot border
            int numDots = 20;  // Adjust this value for the number of dots
            for (int i = 0; i < numDots; i++) {
                float angle = TWO_PI / numDots * i;
                int x1 = x + (radius + 1) * cos(angle);
                int y1 = y + (radius + 1) * sin(angle);
                a.drawPixel(x1, y1, WHITE);
            }
        }
};
Goal levelGoal(100,50,5);

void drawObjects() {
    // Draw all physics/level objects to screen
    for (int i = 0; i < MAX_PLANKS; i++) {
        planks[i].draw();
    }
    newBall.draw();
    levelGoal.draw();
}

void playGame() {
    switch(levelState) {
        case LevelState::Setup:
            a.print("Level Setup\n");
            
            planks[0] = Plank(10, 40, 100, 58, 2); // Diagonal
            planks[1] = Plank(80, 15, 80, 25, 5); // Vertical (x1==x2)


            if (a.justPressed(A_BUTTON)) {
                levelState = LevelState::Play;
            }
            break;
        
        case LevelState::Play:

            if (levelGoal.isBallInside(newBall)) {
                    Serial.print("Inside!");
                    levelState = LevelState::LevelWin;
                }
            newBall.update(planks, MAX_PLANKS);
            break;
        
        case LevelState::LevelWin:
            a.print("Level Clear!");
            break;
        case LevelState::LevelLose:
            a.print("You Lose!");
            break;
    }

    drawObjects();

}

void setup() {
    a.begin();
    a.setFrameRate(FRAME_RATE);
    a.initRandomSeed();
    a.clear();
}

void loop() {
    if (!(a.nextFrame())) {
        return;
    }
    a.pollButtons();
    a.clear();

    switch(gameState) {
        case GameState::Title:
            a.print("Main Title\n\nA to Play\nB for Instructions");
            if (a.justPressed(A_BUTTON)) {
                gameState = GameState::PlayGame;
            }
            if (a.justPressed(B_BUTTON)) {
                gameState = GameState::Instructions;
            }
            break;
        case GameState::Instructions:
            a.print("Instructions");
            if (a.justPressed(B_BUTTON)) {
                gameState = GameState::Title;
            }
            break;
        case GameState::PlayGame:
            playGame();
            break;
        case GameState::EndScreen:
            a.print("End Screen");
            break;
    }
    
    a.display();
}