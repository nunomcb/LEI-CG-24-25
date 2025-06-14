#ifndef OPENGLSPOTLIGHT_H
#define OPENGLSPOTLIGHT_H

#include "OpenGlLight.h"

class OpenGlSpotLight : public OpenGlLight {
private:
    GLenum light_id;
    float pos_x, pos_y, pos_z;
    float dir_x, dir_y, dir_z;
    float r, g, b;
    float cutoff;
    float exponent;

public:
    OpenGlSpotLight(
        GLenum light_id,
        float pos_x, float pos_y, float pos_z,
        float dir_x, float dir_y, float dir_z,
        float r, float g, float b,
        float cutoff,
        float exponent
    );

    ~OpenGlSpotLight() override = default;

    void setup() const override;
    void update() const override;
    bool is_enabled() const override;
};

#endif