#ifndef OPENGLCAMERA_H
#define OPENGLCAMERA_H

#include "../config.h"
#include "model/Vector.h"
#include "Mat4x4.h"

struct OpenGlCameraProjection {
    int fov, near, far;
};

class OpenGlCamera {
public:
    explicit OpenGlCamera(config::Camera camera_config);

    Vector position{};
    Vector look_at{};
    Vector up{};

    OpenGlCameraProjection projection{};

    void move_forward(int speed);
    void move_right(int speed);
    void move_back(int speed);
    void move_left(int speed);

    void look_up(float angle);
    void look_down(float angle);
    void look_left(float angle_rad);
    void look_right(float angle_rad);

    void zoom_in(float factor);
    void zoom_out(float factor);

    void update_projection(int width, int height);

    Mat4x4 get_view_matrix();
    Mat4x4 get_projection_matrix() const;
    Mat4x4 get_inverse_view_matrix();

    bool should_update() const;
private:
    Mat4x4 view_matrix{};
    Mat4x4 inverse_view_matrix;
    Mat4x4 projection_matrix{};
    bool should_update_view = true;

    [[nodiscard]] Vector get_move_vec() const;
    [[nodiscard]] Vector get_dir_vec() const;

    void update_view();
    void set_dirty(bool is_dirty);
};

#endif
