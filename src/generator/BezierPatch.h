#ifndef BEZIERPATCH_H
#define BEZIERPATCH_H

#include <vector>
#include <array>

#include "model/Point.h"

struct BezierPatch {
public:
    std::array<std::array<Point, 4>, 4> control_points;

    BezierPatch() = default;

    explicit BezierPatch(const std::array<std::array<Point, 4>, 4>& points);
    explicit BezierPatch(const std::vector<Point>& flat_points);

    [[nodiscard]] const Point& get_control_point(int u_index, int v_index) const;
    [[nodiscard]] Point point(float u, float v) const;
    [[nodiscard]] Vector normal(float u, float v) const;
private:
    static std::array<float, 4> bernstein3(float t);
    static std::array<float, 3> bernstein2(float t);

    [[nodiscard]] Vector evaluate_derivative_u(float u, float v) const;
    [[nodiscard]] Vector evaluate_derivative_v(float u, float v) const;
};

#endif //BEZIERPATCH_H
