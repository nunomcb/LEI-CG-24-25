#ifndef OPENGLLIGHT_H
#define OPENGLLIGHT_H

#include <GL/glew.h>

class OpenGlLight {
public:
    virtual ~OpenGlLight() = default;

    virtual void setup() const = 0;
    virtual void update() const = 0;
    virtual bool is_enabled() const = 0;
};

#endif