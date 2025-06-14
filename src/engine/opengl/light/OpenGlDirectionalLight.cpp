#include "OpenGlDirectionalLight.h"

#include <GL/glew.h>

OpenGlDirectionalLight::OpenGlDirectionalLight(
    GLenum light_id,
    float dir_x, float dir_y, float dir_z,
    float r, float g, float b
) :
    light_id(light_id),
    dir_x(dir_x),
    dir_y(dir_y),
    dir_z(dir_z),
    r(r),
    g(g),
    b(b) {}

void OpenGlDirectionalLight::setup() const {
    glEnable(light_id);

    GLfloat light_color_diffuse_specular[] = {r, g, b, 1.0f};
    GLfloat light_color_ambient[] = {r * 0.2f, g * 0.2f, b * 0.2f, 1.0f};

    glLightfv(light_id, GL_AMBIENT, light_color_ambient);
    glLightfv(light_id, GL_DIFFUSE, light_color_diffuse_specular);
    glLightfv(light_id, GL_SPECULAR, light_color_diffuse_specular);
}

void OpenGlDirectionalLight::update() const {
    const GLfloat position[] = {dir_x, dir_y, dir_z, 0.0f};

    glLightfv(light_id, GL_POSITION, position);
}

bool OpenGlDirectionalLight::is_enabled() const {
    return true;
}