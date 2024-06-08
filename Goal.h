#ifndef GOAL_H
#define GOAL_H

class Goal {
public:
    Goal(int x, int y, int radius) : x(x), y(y), radius(radius) {}

    void draw() {
        float circumference = 2 * PI * radius;
        float segmentLength = dashLength + dashGap;
        int numberOfDashes = circumference / segmentLength;
        
        for (int i = 0; i < numberOfDashes; ++i) {
            // Calculate start angle of the dash
            float startAngle = (i * segmentLength) / radius;
            // Calculate end angle of the dash
            float endAngle = ((i * segmentLength) + dashLength) / radius;
            
            // Convert start and end angles to x and y coordinates
            int startX = x + radius * cos(startAngle);
            int startY = y + radius * sin(startAngle);
            int endX = x + radius * cos(endAngle);
            int endY = y + radius * sin(endAngle);
            
            // Draw the dash
            a.drawLine(startX, startY, endX, endY, WHITE);
        }
    }

private:
    int x;
    int y;
    int radius;
    int dashLength = 1;
    int dashGap = 3;

};



#endif // GOAL_H