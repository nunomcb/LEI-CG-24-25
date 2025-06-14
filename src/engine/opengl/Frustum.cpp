#include "Frustum.h"

#include <algorithm>
#include <cmath>
#include <GL/glew.h>

Plane::Plane(const Vector& n, float d) {
    const float len = n.length();

    normal = n.normalise();
    distance = d / len;
}

float Plane::get_signed_distance_to_point(const Point& point) const {
    return normal.dot({point.x, point.y, point.z}) + distance;
}

Frustum::Frustum(const Mat4x4& vp_matrix, const Mat4x4& inverse_view_matrix) :
    inverse_view_matrix(inverse_view_matrix),
    left({
        Vector{
            vp_matrix[0][3] + vp_matrix[0][0],
            vp_matrix[1][3] + vp_matrix[1][0],
            vp_matrix[2][3] + vp_matrix[2][0] 
        },
        vp_matrix[3][3] + vp_matrix[3][0]
    }),
    right({
        Vector{
            vp_matrix[0][3] - vp_matrix[0][0],
            vp_matrix[1][3] - vp_matrix[1][0],
            vp_matrix[2][3] - vp_matrix[2][0]
        },
        vp_matrix[3][3] - vp_matrix[3][0]
    }),
    bottom({
        Vector{
            vp_matrix[0][3] + vp_matrix[0][1],
            vp_matrix[1][3] + vp_matrix[1][1],
            vp_matrix[2][3] + vp_matrix[2][1]
        },
        vp_matrix[3][3] + vp_matrix[3][1]
    }),
    top({
        Vector{
            vp_matrix[0][3] - vp_matrix[0][1],
            vp_matrix[1][3] - vp_matrix[1][1],
            vp_matrix[2][3] - vp_matrix[2][1]
        },
        vp_matrix[3][3] - vp_matrix[3][1]
    }),
    near({
        Vector{
            vp_matrix[0][3] + vp_matrix[0][2],
            vp_matrix[1][3] + vp_matrix[1][2],
            vp_matrix[2][3] + vp_matrix[2][2]
        },
        vp_matrix[3][3] + vp_matrix[3][2]
    }),
    far({
        Vector{
            vp_matrix[0][3] - vp_matrix[0][2],
            vp_matrix[1][3] - vp_matrix[1][2],
            vp_matrix[2][3] - vp_matrix[2][2]
        },
        vp_matrix[3][3] - vp_matrix[3][2]
    }) {}

Frustum::Frustum(const Mat4x4 &view_matrix, const Mat4x4 &proj_matrix, const Mat4x4 &inverse_view_matrix)
    : Frustum(proj_matrix * view_matrix, inverse_view_matrix) {}

std::array<Plane, 6> Frustum::get_planes() const {
    return {left, right, top, bottom, near, far};
}

bool Frustum::is_sphere_inside(const BoundingSphere& sphere) const {
    Mat4x4 model_view_matrix;

    glGetFloatv(GL_MODELVIEW_MATRIX, model_view_matrix.data);

    const Mat4x4 model_world_matrix = inverse_view_matrix * model_view_matrix;
    const BoundingSphere world_sphere = sphere.transform(model_world_matrix);

    auto planes = get_planes();

    return std::all_of(planes.begin(), planes.end(), [&](const Plane& plane) {
        return plane.get_signed_distance_to_point(world_sphere.center) >= -world_sphere.radius;
    });
}
