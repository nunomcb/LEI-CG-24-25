#ifndef MAT4X4_H
#define MAT4X4_H

#include "model/Vector.h"
#include "model/Point.h"

struct Mat4x4 {
    float data[16];
    Point operator * (const Point& p) const;
    Mat4x4 operator * (const Mat4x4& other) const;

    float* operator[](int col_idx);
    const float* operator[](int col_idx) const;

    float get_max_scale_factor() const;
    float determinant() const;
    Mat4x4 inverse() const;
};

#endif
