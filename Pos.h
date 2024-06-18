#ifndef POS_H
#define POS_H

template <typename T>
class Pos {
public:
    T x;
    T y;

    // Constructors
    Pos() : x(0), y(0) {}
    Pos(T x, T y) : x(x), y(y) {}

    // Setter
    void set(T newX, T newY) {
        this->x = newX;
        this->y = newY;
    }

    // Arithmetic operations
    Pos operator+(const Pos& other) const {
        return Pos(x + other.x, y + other.y);
    }

    Pos operator-(const Pos& other) const {
        return Pos(x - other.x, y - other.y);
    }

    template <typename U>
    Pos<T> operator-(const Pos<U>& other) const {
        return Pos<T>(x - static_cast<T>(other.x), y - static_cast<T>(other.y));
    }

    // Compound assignment operators
    Pos& operator+=(const Pos& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Pos& operator-=(const Pos& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Equality operators
    bool operator==(const Pos& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Pos& other) const {
        return !(*this == other);
    }
};

#endif // POS_H