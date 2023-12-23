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

        // Define operator - for subtracting two Pos of any type (i.e. float and uint8_t)
        template <typename U>
        Pos<T> operator-(const Pos<U>& other) const {
            return Pos<T>(x - static_cast<T>(other.x), y - static_cast<T>(other.y));
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

        // Dot product method
        float dot(const Vector& otherVector) const {
            return (dx * otherVector.dx) + (dy * otherVector.dy);
        }

        // Scalar multiplication
        Vector operator*(float scalar) const {
            return Vector(dx * scalar, dy * scalar);
        }

        // In-place scalar multiplication
        Vector& operator*=(float scalar) {
            dx *= scalar;
            dy *= scalar;
            return *this;
        }

        // Vector subtraction
        Vector operator-(const Vector& other) const {
            return Vector(dx - other.dx, dy - other.dy);
        }

        // In-place vector subtraction
        Vector& operator-=(const Vector& other) {
            dx -= other.dx;
            dy -= other.dy;
            return *this;
        }

};


#endif