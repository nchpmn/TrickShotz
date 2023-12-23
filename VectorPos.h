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

        // Define operator+= for adding a Vector to Pos
        Pos<T>& operator+=(const Vector& vector) {
            x += vector.dx;
            y += vector.dy;
            return *this;
        }
};


// VECTOR CLASS
class Vector {
    public:
        float dx;
        float dy;

        // Default constructor with initialization
        Vector() : dx(0), dy(0) {}

        Vector(float dxComponent, float dyComponent) : dx(dxComponent), dy(dyComponent) {}
};


#endif