#include "Point.h"

Point::Point(float x, float y, float z) : x(x), y(y), z(z) {}

Point::Point(): Point(0.0f, 0.0f, 0.0f) {}
Point::Point(const Vector &v) : Point(v.x, v.y, v.z) {}

Vector Point::operator -(const Point& other) const {
    return { x - other.x, y - other.y, z - other.z };
}

Point Point::operator +(const Vector& v) const {
    return {x + v.x, y + v.y, z + v.z};
}

Point Point::operator -(const Vector& v) const {
    return {x - v.x, y - v.y, z - v.z};
}

Point Point::operator *(const float scalar) const {
    return { x * scalar, y * scalar, z * scalar };
}

bool Point::operator <(const Point& other) const {
    return x < other.x
        || x == other.x && y < other.y
        || x == other.x && y == other.y && z < other.z;
}

Vector Point::to_vector() const {
    return {x, y, z};
}
