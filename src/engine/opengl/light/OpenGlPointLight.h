#ifndef OPENGLPOINTLIGHT_H
#define OPENGLPOINTLIGHT_H

#include "OpenGlLight.h"

class OpenGlPointLight : public OpenGlLight {
private:
    GLenum light_id;
    float pos_x, pos_y, pos_z;
    float r, g, b;

public:
    OpenGlPointLight(
        GLenum light_id,
        float pos_x, float pos_y, float pos_z,
        float r, float g, float b
    );

    ~OpenGlPointLight() override = default;

    void setup() const override;
    void update() const override;
    bool is_enabled() const override;
};

#endif