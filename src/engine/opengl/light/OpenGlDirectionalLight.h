#ifndef OPENGLDIRECTIONALLIGHT_H
#define OPENGLDIRECTIONALLIGHT_H

#include "OpenGlLight.h"

class OpenGlDirectionalLight : public OpenGlLight {
private:
    GLenum light_id;
    float dir_x, dir_y, dir_z;
    float r, g, b;

public:
    OpenGlDirectionalLight(
        GLenum light_id,
        float dir_x, float dir_y, float dir_z,
        float r, float g, float b
    );

    ~OpenGlDirectionalLight() override = default;

    void setup() const override;
    void update() const override;
    bool is_enabled() const override;
};

#endif