#ifndef POS_H
#define POS

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

#endif;