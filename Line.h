#ifndef LINE_H
#define LINE_H

class Line {
public:
    Line() : startPos(0, 0), endPos(0, 0) {} // Default constructor

    Line(int x1, int y1, int x2, int y2) : startPos(x1, y1), endPos(x2, y2) {}
    
    void draw() {
        if (startPos.x == endPos.x) {
            // Case where line is vertical
            a.fillRect(startPos.x, startPos.y, thickness, endPos.y - startPos.y, WHITE);
        } else if (startPos.y == endPos.y) {
            // Case where line is horizontal
            a.fillRect(startPos.x, startPos.y, endPos.x - startPos.x, thickness, WHITE);
        } else {
            // Case where line is diagonal
            if (thickness % 2 == 0) {
                uint8_t halfThick = thickness / 2;
                a.fillTriangle(startPos.x, startPos.y - halfThick, endPos.x, endPos.y - halfThick, startPos.x, startPos.y + halfThick, WHITE);
                a.fillTriangle(endPos.x, endPos.y - halfThick, endPos.x, endPos.y + halfThick, startPos.x, startPos.y + halfThick, WHITE);
            } else {
                a.fillTriangle(startPos.x, startPos.y, endPos.x, endPos.y, startPos.x, startPos.y + thickness, WHITE);
                a.fillTriangle(endPos.x, endPos.y, endPos.x, endPos.y + thickness, startPos.x, startPos.y + thickness, WHITE);
            }
        }
    }

    Pos<uint8_t> getStartPos() const { return startPos; }
    Pos<uint8_t> getEndPos() const { return endPos; }
    int getX1() const { return startPos.x; }
    int getY1() const { return startPos.y; }
    int getX2() const { return endPos.x; }
    int getY2() const { return endPos.y; }
    int getThick() const { return thickness; }
    void setPoints(uint8_t newX1, uint8_t newY1, uint8_t newX2, uint8_t newY2) {
        startPos.set(newX1, newY1);
        endPos.set(newX2, newY2);
    }

private:
    Pos<uint8_t> startPos;
    Pos<uint8_t> endPos;
    static const int thickness = 2;

};

#endif // LINE_H