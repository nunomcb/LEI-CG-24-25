#include "OpenGlCamera.h"

#include <cmath>
#include <GL/glew.h>

OpenGlCamera::OpenGlCamera(const config::Camera camera_config) {
    position = camera_config.position.asVector();
    look_at = camera_config.look_at.asVector();
    up = camera_config.up.asVector();

    projection = {
        camera_config.projection.fov,
        camera_config.projection.near,
        camera_config.projection.far,
    };
}

void OpenGlCamera::set_dirty(bool is_dirty) {
    should_update_view = is_dirty;
}

bool OpenGlCamera::should_update() const {
    return should_update_view;
}

Vector OpenGlCamera::get_dir_vec() const {
    return (look_at - position).normalise();
}

Vector OpenGlCamera::get_move_vec() const {
    Vector dir_vec = get_dir_vec();

    return {dir_vec.x, 0, dir_vec.z};
}

void OpenGlCamera::zoom_in(const float factor) {
    auto dir_vec = get_dir_vec();

    position = position + dir_vec * factor;

    set_dirty(true);
}

void OpenGlCamera::zoom_out(const float factor) {
    auto dir_vec = get_dir_vec();

    position = position - dir_vec * factor;

    set_dirty(true);
}

void OpenGlCamera::move_forward(const int speed) {
    auto move_vec = get_move_vec();

    position = position + move_vec * (speed * 0.2f);
    look_at = look_at + move_vec * (speed * 0.2f);

    set_dirty(true);
}

void OpenGlCamera::move_back(const int speed) {
    auto move_vec = get_move_vec();

    position = position - move_vec * (speed * 0.2f);
    look_at = look_at - move_vec * (speed * 0.2f);

    set_dirty(true);
}

void OpenGlCamera::move_left(int speed) {
    auto move_vec = get_move_vec();
    auto left_vec = Vector{0, 1, 0}.cross(move_vec);

    position = position + left_vec * (speed * 0.2f);
    look_at = look_at + left_vec * (speed * 0.2f);

    set_dirty(true);
}

void OpenGlCamera::move_right(int speed) {
    auto move_vec = get_move_vec();
    auto right_vec = move_vec.cross(Vector{0, 1, 0});

    position = position + right_vec * (speed * 0.2f);
    look_at = look_at + right_vec * (speed * 0.2f);

    set_dirty(true);
}

static Vector rotate_vector_around_axis(const Vector& vec, const Vector& axis, float angle_rad) {
    Vector k = axis.normalise();
    float cos_theta = std::cos(angle_rad);
    float sin_theta = std::sin(angle_rad);
    
    Vector term1 = vec * cos_theta;
    Vector term2 = k.cross(vec) * sin_theta;
    Vector term3 = k * k.dot(vec) * (1.0f - cos_theta);

    return term1 + term2 + term3;
}

void OpenGlCamera::look_up(float angle_rad) {
    Vector view_dir = (look_at - position);
    Vector right_vec = view_dir.cross(up).normalise();
    Vector new_view_dir = rotate_vector_around_axis(view_dir, right_vec, angle_rad);

    float dot_with_world_up = new_view_dir.normalise().dot(Vector{0.0f, 1.0f, 0.0f});
    float max_angle_from_horizontal = 89.0f * static_cast<float>(M_PI) / 180.0f;
    
    if (std::acos(dot_with_world_up) > (static_cast<float>(M_PI_2) + max_angle_from_horizontal)) {
        return;
    }

    look_at = position + new_view_dir;

    set_dirty(true);
}

void OpenGlCamera::look_down(float angle_rad) {
    Vector view_dir = (look_at - position);
    Vector right_vec = view_dir.cross(up).normalise();

    Vector new_view_dir = rotate_vector_around_axis(view_dir, right_vec, -angle_rad);

    float dot_with_world_down = new_view_dir.normalise().dot(Vector{0.0f, -1.0f, 0.0f});
    float max_angle_from_horizontal = 89.0f * static_cast<float>(M_PI) / 180.0f;

    if (std::acos(dot_with_world_down) > (static_cast<float>(M_PI_2) + max_angle_from_horizontal)) {
        return;
    }

    look_at = position + new_view_dir;

    set_dirty(true);
}

void OpenGlCamera::look_left(float angle_rad) {
    Vector view_dir = look_at - position;
    Vector new_view_dir = rotate_vector_around_axis(view_dir, up, angle_rad);

    look_at = position + new_view_dir;

    set_dirty(true);
}

void OpenGlCamera::look_right(float angle_rad) {
    Vector view_dir = look_at - position;
    Vector new_view_dir = rotate_vector_around_axis(view_dir, up, -angle_rad);

    look_at = position + new_view_dir;

    set_dirty(true);
}


void OpenGlCamera::update_view() {
    if (!should_update_view) {
        return;
    }

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    gluLookAt(
        position.x, position.y, position.z,
        look_at.x, look_at.y, look_at.z,
        up.x, up.y, up.z
    );

    glGetFloatv(GL_MODELVIEW_MATRIX, view_matrix.data);

    glPopMatrix();
    
    inverse_view_matrix = view_matrix.inverse();
    
    set_dirty(false); 
}

void OpenGlCamera::update_projection(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const float ratio =  width * 1.0f / height;

    glViewport(0, 0, width, height);
    gluPerspective(
        projection.fov,
        ratio,
        projection.near,
        projection.far
    );

    glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix.data);

    glMatrixMode(GL_MODELVIEW);
}

Mat4x4 OpenGlCamera::get_view_matrix() {
    update_view();

    return view_matrix;
}

Mat4x4 OpenGlCamera::get_projection_matrix() const {
    return projection_matrix;
}

Mat4x4 OpenGlCamera::get_inverse_view_matrix() {
    update_view();

    return inverse_view_matrix;
}