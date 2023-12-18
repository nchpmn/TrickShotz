#ifndef VECTORPOS_H
#define VECTORPOS

class Position {
    public:
        float x;
        float y;
        Position(float x, float y) : x(x), y(y) {}
};

class Vector {
    public:
        float x;
        float y;
        Vector(float xComponent, float yComponent) : x(xComponent), y(yComponent) {}
};


#endif