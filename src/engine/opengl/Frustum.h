#ifndef CG_FRUSTUM_H
#define CG_FRUSTUM_H

#include "model/Point.h"
#include "model/Vector.h"
#include "BoundingSphere.h"
#include "Mat4x4.h"

#include <array>

struct Plane {
    Vector normal;
    float distance;

    Plane() : normal(), distance(0.0f) {}
    Plane(const Vector& n, float d);

    float get_signed_distance_to_point(const Point& point) const;
};

class Frustum {
public:
    Frustum(const Mat4x4& vp_matrix, const Mat4x4& inverse_view_matrix);
    Frustum(const Mat4x4& view_matrix, const Mat4x4& proj_matrix, const Mat4x4& inverse_view_matrix);

    bool is_sphere_inside(const BoundingSphere& sphere) const;

private:
    Plane left, right, top, bottom, near, far;
    Mat4x4 inverse_view_matrix;

    std::array<Plane, 6> get_planes() const;
};

#endif //CG_FRUSTUM_H