#include "BezierPatch.h"

BezierPatch::BezierPatch(const std::array<std::array<Point, 4>, 4>& points)
    : control_points(points) {}

BezierPatch::BezierPatch(const std::vector<Point>& flat_points) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            control_points[i][j] = flat_points[i * 4 + j];
        }
    }
}

const Point& BezierPatch::get_control_point(int u_index, int v_index) const {
    return control_points[u_index][v_index];
}

std::array<float, 4> BezierPatch::bernstein3(float t) {
    const float one_minus_t = 1.0f - t;

    return std::array<float, 4>{
        one_minus_t * one_minus_t * one_minus_t,
        one_minus_t * one_minus_t * t * 3.0f,
        one_minus_t * t * t * 3.0f,
        t * t * t
    };
}

std::array<float, 3> BezierPatch::bernstein2(float t) {
    const float one_minus_t = 1.0f - t;

    return std::array<float, 3>{
        one_minus_t * one_minus_t,
        one_minus_t * t * 2.0f,
        t * t
    };
}

Point BezierPatch::point(float u, float v) const {
    Point result = {0.0f, 0.0f, 0.0f};

    const auto bu = bernstein3(u);
    const auto bv = bernstein3(v);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            const float weight = bu[i] * bv[j];
            const Point& p = control_points[i][j];

            result.x += weight * p.x;
            result.y += weight * p.y;
            result.z += weight * p.z;
        }
    }

    return result;
}

Vector BezierPatch::evaluate_derivative_u(float u, float v) const {
    Vector result_u = {0.0f, 0.0f, 0.0f};

    auto bv_deg3 = bernstein3(v);
    auto bu_deg2 = bernstein2(u);

    for (int j = 0; j < 4; ++j) {
        for (int i = 0; i < 3; ++i) {
            Vector diff = control_points[i + 1][j] - control_points[i][j];

            float weight = bu_deg2[i] * bv_deg3[j];
            result_u = result_u + (diff * weight);
        }
    }

    return result_u * static_cast<float>(3);
}

Vector BezierPatch::evaluate_derivative_v(float u, float v) const {
    Vector result_v = {0.0f, 0.0f, 0.0f};

    auto bu_deg3 = bernstein3(u);
    auto bv_deg2 = bernstein2(v);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            Vector diff = control_points[i][j + 1] - control_points[i][j];

            float weight = bu_deg3[i] * bv_deg2[j];
            result_v = result_v + (diff * weight);
        }
    }

    return result_v * static_cast<float>(3);
}

Vector BezierPatch::normal(float u, float v) const {
    Vector du = evaluate_derivative_u(u, v);
    Vector dv = evaluate_derivative_v(u, v);

    return dv.cross(du).normalise();
}
