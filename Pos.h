#ifndef POS_H
#define POS_H

class Pos {
public:
    int x;
    int y;

    // Constructors
    Pos() : x(0), y(0) {}
    Pos(int x, int y) : x(x), y(y) {}

    // Setters
    void set(int x, int y) {
        this->x = x;
        this->y = y;
    }

    // Arithmetic operations
    Pos operator+(const Pos& other) const {
        return Pos(x + other.x, y + other.y);
    }

    Pos operator-(const Pos& other) const {
        return Pos(x - other.x, y - other.y);
    }

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

    bool operator==(const Pos& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Pos& other) const {
        return !(*this == other);
    }
};

#endif // POS_H