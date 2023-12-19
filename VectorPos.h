#ifndef VECTORPOS_H
#define VECTORPOS

// POSITION CLASS
// Uses templates to work for various int/float types
template <typename T>
class Position {
    public:
        T x;
        T y;
        Position(T x, T y) : x(x), y(y) {}
};

class Vector {
    public:
        float x;
        float y;
        Vector(float xComponent, float yComponent) : x(xComponent), y(yComponent) {}
};


#endif