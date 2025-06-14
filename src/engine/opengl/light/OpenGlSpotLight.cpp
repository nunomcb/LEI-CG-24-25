#include "OpenGlSpotLight.h"
#include <GL/glew.h>

OpenGlSpotLight::OpenGlSpotLight(
    GLenum light_id,
    float pos_x, float pos_y, float pos_z,
    float dir_x, float dir_y, float dir_z,
    float r, float g, float b,
    float cutoff,
    float exponent
) :
    light_id(light_id),
    pos_x(pos_x),
    pos_y(pos_y),
    pos_z(pos_z),
    dir_x(dir_x),
    dir_y(dir_y),
    dir_z(dir_z),
    r(r),
    g(g),
    b(b),
    cutoff(cutoff),
    exponent(exponent) {}

void OpenGlSpotLight::setup() const {
    glEnable(light_id);

    GLfloat white[4] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(light_id, GL_AMBIENT, white);
    glLightfv(light_id, GL_DIFFUSE, white);
    glLightfv(light_id, GL_SPECULAR, white);

    glLightf(light_id, GL_SPOT_CUTOFF, cutoff);
    glLightf(light_id, GL_SPOT_EXPONENT, exponent);
}

void OpenGlSpotLight::update() const {
    GLfloat position[] = {pos_x, pos_y, pos_z, 1.0f};
    GLfloat direction[] = {dir_x, dir_y, dir_z};

    glLightfv(light_id, GL_POSITION, position);
    glLightfv(light_id, GL_SPOT_DIRECTION, direction);
}

bool OpenGlSpotLight::is_enabled() const {
    return true;
}