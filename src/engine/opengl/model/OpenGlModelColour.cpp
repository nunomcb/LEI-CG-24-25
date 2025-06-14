#include "OpenGlModelColour.h"

#include <cstring>

OpenGlModelColour::OpenGlModelColour(
    const GLfloat diffuse[4],
    const GLfloat ambient[4],
    const GLfloat specular[4],
    const GLfloat emissive[4],
    const GLfloat shininess
) :
    shininess(shininess)
{
    std::memcpy(this->diffuse, diffuse, sizeof(GLfloat) * 4);
    std::memcpy(this->ambient, ambient, sizeof(GLfloat) * 4);
    std::memcpy(this->specular, specular, sizeof(GLfloat) * 4);
    std::memcpy(this->emissive, emissive, sizeof(GLfloat) * 4);
}

void OpenGlModelColour::apply() const {
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
