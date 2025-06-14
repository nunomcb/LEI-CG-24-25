#ifndef CG_VECTOR_H
#define CG_VECTOR_H

struct Vector {
public:
    float x, y, z;

    Vector operator + (const Vector& other) const;
    Vector operator - (const Vector& other) const;
    Vector operator * (float scalar) const;
    Vector operator * (int scalar) const;
    Vector operator / (float scalar) const;
    Vector operator - () const;

    bool operator < (const Vector& other) const;

    [[nodiscard]] float dot(const Vector& other) const;
    [[nodiscard]] Vector cross(const Vector& other) const;
    [[nodiscard]] Vector normalise() const;

    [[nodiscard]] float length() const;
    [[nodiscard]] float length_squared() const;
};

#endif //CG_VECTOR_H
