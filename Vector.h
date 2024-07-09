#ifndef VECTOR_H
#define VECTOR_H

class Vector {
public:
    float dx;
    float dy;

    // Constructors
    Vector() : dx(0), dy(0) {}
    Vector(float dx, float dy) : dx(dx), dy(dy) {}
    Vector(Pos<float> pos) : dx(pos.x), dy(pos.y) {}
    Vector(Pos<uint8_t> pos) : dx(pos.x), dy(pos.y) {}
    Vector(Pos<int16_t> pos) : dx(pos.x), dy(pos.y) {}

    // Setter
    void set(float newDx, float newDy) {
        this->dx = newDx;
        this->dy = newDy;
    }

    // Calculate the magnitude of the vector
    float magnitude() const {
        return sqrt(dx * dx + dy * dy);
    }

    float magnitudeSquared() const {
        return dx * dx + dy * dy;
    }

    // Dot product
    float dot(const Vector& other) const {
        return dx * other.dx + dy * other.dy;
    }

    // Normalize the vector
    Vector normalise() const {
        float mag = magnitude();
        return Vector(dx / mag, dy / mag);
    }
    

    // Assignment operator
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            dx = other.dx;
            dy = other.dy;
        }
        return *this;
    }

    // Arithmetic operations
    Vector operator+(const Vector& other) const {
        return Vector(dx + other.dx, dy + other.dy);
    }

    Vector operator-(const Vector& other) const {
        return Vector(dx - other.dx, dy - other.dy);
    }

    Vector operator*(float scalar) const {
        return Vector(dx * scalar, dy * scalar);
    }

    Vector operator/(float scalar) const {
        return Vector(dx / scalar, dy / scalar);
    }

    // Compound assignment operators
    Vector& operator+=(const Vector& other) {
        dx += other.dx;
        dy += other.dy;
        return *this;
    }

    Vector& operator-=(const Vector& other) {
        dx -= other.dx;
        dy -= other.dy;
        return *this;
    }

    Vector& operator*=(float scalar) {
        dx *= scalar;
        dy *= scalar;
        return *this;
    }

    Vector& operator/=(float scalar) {
        dx /= scalar;
        dy /= scalar;
        return *this;
    }

    // Equality operators
    bool operator==(const Vector& other) const {
        return (dx == other.dx && dy == other.dy);
    }

    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }
};

#endif // VECTOR_H
