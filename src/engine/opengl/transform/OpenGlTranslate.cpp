#include <GL/glew.h>

#include "OpenGlTranslate.h"

OpenGlTranslate::OpenGlTranslate(float x, float y, float z) :
    x(x), y(y), z(z) {}

void OpenGlTranslate::apply() {
    glTranslatef(x, y, z);
}