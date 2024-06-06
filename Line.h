#ifndef LINE_H
#define LINE_H

class Line {
public:
    Line(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
    
    void draw() {
        a.drawLine(x1, y1, x2, y2);
    }

    int getX1() const { return x1; }
    int getY1() const { return y1; }
    int getX2() const { return x2; }
    int getY2() const { return y2; }

private:
    int x1;
    int y1;
    int x2;
    int y2;

};

#endif // LINE_H