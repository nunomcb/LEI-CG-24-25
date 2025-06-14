#include "BoundingSphere.h"

#include <cmath>

BoundingSphere::BoundingSphere(const std::vector<Point>& positions) {
    Vector sum_positions = {0.0f, 0.0f, 0.0f};

    for (auto position : positions) {
        sum_positions = position.to_vector() + sum_positions;
    }

    center = Point(sum_positions / positions.size());

    float max_radius_sq = 0.0f;

    for (auto position : positions) {
        Vector diff = position - center;

        if (diff.length_squared() > max_radius_sq) {
            max_radius_sq = diff.length_squared();
        }
    }

    radius = std::sqrt(max_radius_sq);
}

BoundingSphere BoundingSphere::transform(const Mat4x4& matrix) const {
    return BoundingSphere{
        matrix * center,
        this->radius * matrix.get_max_scale_factor()
    };
}