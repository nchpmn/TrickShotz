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

class Plank {
    public:
        int16_t x1, y1; // Coordinates of one end
        int16_t x2, y2; // Coordinates of the other end
        uint8_t thickness; // Thickness of the plank

        // Default constructor with default values
        Plank() : x1(0), y1(0), x2(0), y2(0), thickness(1) {}


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

        void update(Plank planks[], int numPlanks) {
            // Gravity applied to ball
            vy += GRAVITY;
            
            // Update the ball's position from it's velocity values
            x += vx;
            y += vy;

            // Check for collision with planks
            for (int i = 0; i < MAX_PLANKS; i++) {
                if (planks[i].checkCollision(x, y, size)) {
                    // Collision detected with plank[i]
                    // Bounce!
                    vx = -vx;
                    vy = -vy;
                }
            }
        }

        void draw() {
            a.fillCircle(static_cast<int16_t>(round(x)), static_cast<int16_t>(round(y)), size, WHITE);
        }
};
Ball newBall(10,20,0.5,-0.5,2);

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
    End
};
LevelState levelState = LevelState::Setup;


void playGame() {
    switch(levelState) {
        case LevelState::Setup:
            a.print("Level Setup\n");
            
            planks[0] = Plank(80, 10, 80, 40, 5); // Vertical (x1==x2)
            planks[1] = Plank(10, 40, 100, 58, 2); // Diagonal

            if (a.justPressed(A_BUTTON)) {
                levelState = LevelState::Play;
            }

            break;
        
        case LevelState::Play:
            newBall.update(planks, MAX_PLANKS);

            break;
        
        case LevelState::End:
            a.print("Level Clear!");
    }

    for (int i = 0; i < MAX_PLANKS; i++) {
        planks[i].draw();
    }
    newBall.draw();
}

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