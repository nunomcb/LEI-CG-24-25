#ifndef CG_BOUNDINGSPHERE_H
#define CG_BOUNDINGSPHERE_H

#include "Mat4x4.h"
#include "model/Point.h"

#include <vector>

class BoundingSphere {
public:
    Point center;
    float radius;

    BoundingSphere() : center(), radius(0.0f) {}
    BoundingSphere(const std::vector<Point>& points);
    BoundingSphere(Point center, float radius) : center(center), radius(radius) {}

    BoundingSphere transform(const Mat4x4& matrix) const;
};

#endif