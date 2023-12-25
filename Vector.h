#pragma once

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