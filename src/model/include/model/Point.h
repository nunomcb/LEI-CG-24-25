#ifndef CG_POINT_H
#define CG_POINT_H

#include "Vector.h"

struct Point {
    float x, y, z;

    Point();
    Point(float x, float y, float z);
    explicit Point(const Vector& v);

    Vector operator - (const Point& other) const;
    Point operator + (const Vector& v) const;
    Point operator - (const Vector& v) const;
    Point operator * (float scalar) const;

    bool operator < (const Point& other) const;

    Vector to_vector() const;
};

#endif //CG_POINT_H
