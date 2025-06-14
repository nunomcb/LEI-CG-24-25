#include "OpenGlPointLight.h"

#include <GL/glew.h>

OpenGlPointLight::OpenGlPointLight(
    GLenum light_id,
    float pos_x, float pos_y, float pos_z,
    float r, float g, float b
) :
    light_id(light_id),
    pos_x(pos_x),
    pos_y(pos_y),
    pos_z(pos_z),
    r(r),
    g(g),
    b(b) {}

void OpenGlPointLight::setup() const {
    glEnable(light_id);

    GLfloat light_color_diffuse_specular[] = {r, g, b, 1.0f};
    GLfloat light_color_ambient[] = {r * 0.2f, g * 0.2f, b * 0.2f, 1.0f};

    glLightfv(light_id, GL_AMBIENT, light_color_ambient);
    glLightfv(light_id, GL_DIFFUSE, light_color_diffuse_specular);
    glLightfv(light_id, GL_SPECULAR, light_color_diffuse_specular);
}

void OpenGlPointLight::update() const {
    const GLfloat position[] = {pos_x, pos_y, pos_z, 1.0f};

    glLightfv(light_id, GL_POSITION, position);
}

bool OpenGlPointLight::is_enabled() const {
    return true;
}