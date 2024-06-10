#ifndef LINE_H
#define LINE_H

class Line {
public:
    Line() : position1(0, 0), position2(0, 0) {} // Default constructor

    Line(int x1, int y1, int x2, int y2) : position1(x1, y1), position2(x2, y2) {}
    
    void draw() {
        if (getX1() == getX2()) {
            // Case where line is vertical
            a.fillRect(getX1(), getY1(), thickness, getY2()-getY1(), WHITE);
        } else if (getY1() == getY2()) {
            // Case where line is horizontal
            a.fillRect(getX1(), getY1(), getX2()-getX1(), thickness, WHITE);
        } else {
            // Case where line is diagonal
            if (thickness % 2 == 0) {
                uint8_t halfThick = thickness / 2;
                a.fillTriangle(getX1(), getY1()-halfThick, getX2(), getY2()-halfThick, getX1(), getY1()+halfThick, WHITE);
                a.fillTriangle(getX2(), getY2()-halfThick, getX2(), getY2()+halfThick, getX1(), getY1()+halfThick, WHITE);
            } else {
                a.fillTriangle(getX1(), getY1(), getX2(), getY2(), getX1(), getY1()+thickness, WHITE);
                a.fillTriangle(getX2(), getY2(), getX2(), getY2()+thickness, getX1(), getY1()+thickness, WHITE);
            }
        }
    }

    int getX1() const { return position1.x; }
    int getY1() const { return position1.y; }
    int getX2() const { return position2.x; }
    int getY2() const { return position2.y; }
    int getThick() const { return thickness; }
    void setPoints(uint8_t newX1, uint8_t newY1, uint8_t newX2, uint8_t newY2) {
        position1.set(newX1, newY1);
        position2.set(newX2, newY2);
    }

private:
    Pos position1;
    Pos position2;
    static const int thickness = 2;

};

#endif // LINE_H