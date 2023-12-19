#ifndef VECTORPOS_H
#define VECTORPOS

// POSITION CLASS
// Uses templates to work for various int/float types
template <typename T>
class Pos {
    public:
        T x;
        T y;
        Pos(T x, T y) : x(x), y(y) {}
};


// VECTOR CLASS
class Vector {
    public:
        float dx;
        float dy;
        Vector(float dxComponent, float dyComponent) : dx(dxComponent), dy(dyComponent) {}
};


#endif