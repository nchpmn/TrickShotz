#ifndef LINE_H
#define LINE_H

class Line {
public:
    Line(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
    
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

    int getX1() const { return x1; }
    int getY1() const { return y1; }
    int getX2() const { return x2; }
    int getY2() const { return y2; }
    int getThick() const { return thickness; }
    void setPoints(uint8_t newX1, uint8_t newY1, uint8_t newX2, uint8_t newY2) {
        x1 = newX1;
        y1 = newY1;
        x2 = newX2;
        y2 = newY2;
    }

private:
    int x1;
    int y1;
    int x2;
    int y2;
    static const int thickness = 2;

};

#endif // LINE_H