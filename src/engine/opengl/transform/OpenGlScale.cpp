#include <GL/glew.h>

#include "OpenGlScale.h"

OpenGlScale::OpenGlScale(float x, float y, float z) :
    x(x), y(y), z(z) {}

void OpenGlScale::apply() {
    glScalef(x, y, z);
}
