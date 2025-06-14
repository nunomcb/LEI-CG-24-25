#include "OpenGlCatmullRomTranslate.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <algorithm>
#include <array>

OpenGlCatmullRomTranslate::OpenGlCatmullRomTranslate(
    const std::vector<Point>& points, float time_s, bool align, bool show_curve
) :
    control_points(points),
    total_loop_time_s(time_s),
    align_object(align),
    show_curve(show_curve) {}

std::tuple<Point, Vector> OpenGlCatmullRomTranslate::get_point(float t, int *indices) {
    const float basis_matrix[4][4] = {
        { -0.5f, 1.5f, -1.5f, 0.5f },
        { 1.0f, -2.5f, 2.0f, -0.5f },
        { -0.5f, 0.0f, 0.5f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f }
    };

    Point pos = {0.0f, 0.0f, 0.0f};
    Vector der = {0.0f, 0.0f, 0.0f};

    float t_vec[4] = {t*t*t, t*t, t, 1.0f};
    float t_vec_der[4] = {3.0f*t*t, 2.0f*t, 1.0f, 0.0f};

    float pos_weights[4] = {0, 0, 0, 0};
    float der_weights[4] = {0, 0, 0, 0};

    for (int j = 0; j < 4; ++j) {
        for (int i = 0; i < 4; ++i) {
            pos_weights[j] += t_vec[i] * basis_matrix[i][j];
            der_weights[j] += t_vec_der[i] * basis_matrix[i][j];
        }
    }

    for (int i = 0; i < 4; ++i) {
        const Point& pt = control_points[indices[i]];
        float w_pos = pos_weights[i];
        float w_der = der_weights[i];

        pos.x += pt.x * w_pos;
        pos.y += pt.y * w_pos;
        pos.z += pt.z * w_pos;

        der.x += pt.x * w_der;
        der.y += pt.y * w_der;
        der.z += pt.z * w_der;
    }

    return std::make_tuple(pos, der);
}

std::tuple<Point, Vector> OpenGlCatmullRomTranslate::get_global_point(float global_time) {
    int point_count = control_points.size();

    float time_scaled = global_time * point_count;
    int index = static_cast<int>(floor(time_scaled));
    float time_local = time_scaled - index;

    int indices[4] = {
        (index - 1 + point_count) % point_count,
        index % point_count,
        (index + 1) % point_count,
        (index + 2) % point_count
    };

    return get_point(time_local, indices);
}

void OpenGlCatmullRomTranslate::render_curve() {
    Point current_pos;

    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);

    const int steps_per_segment = 20;
    const int total_steps = control_points.size() * steps_per_segment;
    const float step_size = 1.0f / static_cast<float>(total_steps);

    for (int i = 0; i <= total_steps; i++) {
        float t = std::min(static_cast<float>(i) * step_size, 1.0f);
        auto point_data = get_global_point(t);
        current_pos = std::get<0>(point_data);

        glVertex3f(current_pos.x, current_pos.y, current_pos.z);
    }

    glEnd();
    glPopMatrix();
}

void OpenGlCatmullRomTranslate::align(Vector der_arg) {
    Vector world_up = {0.0f, 1.0f, 0.0f};
    Vector der = der_arg.normalise();

    Vector left = world_up.cross(der).normalise();
    Vector up = der.cross(left).normalise();

    // TODO revisit this
    glMultMatrixf(std::array<float, 16>{
        der.x, der.y, der.z, 0.0f,
        left.x, left.y, left.z, 0.0f,
        up.x, up.y, up.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }.data());
}


void OpenGlCatmullRomTranslate::apply() {
    float t = fmod(glutGet(GLUT_ELAPSED_TIME) / (total_loop_time_s * 1000.0f), 1.0f);

    if (show_curve) {
        render_curve();
    };

    auto point_data = get_global_point(t);
    Point current_pos = std::get<0>(point_data);
    Vector current_der = std::get<1>(point_data);

    glTranslatef(current_pos.x, current_pos.y, current_pos.z);

    if (align_object) {
        align(current_der);
    }
}

void OpenGlCatmullRomTranslate::toggle_curve() {
    show_curve = !show_curve;
}
