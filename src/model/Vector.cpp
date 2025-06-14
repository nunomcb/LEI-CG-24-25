#include "Vector.h"

#include <valarray>

bool Vector::operator < (const Vector& other) const {
    return x < other.x
        || x == other.x && y < other.y
        || x == other.x && y == other.y && z < other.z;
}

Vector Vector::operator + (const Vector& other) const {
    return {
        x + other.x,
        y + other.y,
        z + other.z
    };
}

Vector Vector::operator - (const Vector& other) const {
    return {
        x - other.x,
        y - other.y,
        z - other.z
    };
}

Vector Vector::operator * (int n) const {
    return {
        x * float(n),
        y * float(n),
        z * float(n)
    };
}

Vector Vector::operator * (float n) const {
    return {
        x * n,
        y * n,
        z * n
    };
}

float Vector::length() const {
    return std::sqrt(length_squared());
}

float Vector::length_squared() const {
    return x * x + y * y + z * z;
}

Vector operator * (float scalar, const Vector& v) {
    return v * scalar;
}

Vector Vector::operator / (float scalar) const {
    float inv_scalar = 1.0f / scalar;
    return {x * inv_scalar, y * inv_scalar, z * inv_scalar};
}

Vector Vector::operator - () const {
    return {-x, -y, -z};
}

float Vector::dot(const Vector& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector Vector::cross(const Vector &other) const {
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

Vector Vector::normalise() const {
    float vector_len = length();

    return {x / vector_len, y / vector_len, z / vector_len};
}
