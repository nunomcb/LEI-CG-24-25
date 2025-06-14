#ifndef OPENGLMODELCOLOUR_H
#define OPENGLMODELCOLOUR_H

#include <GL/glew.h>

class OpenGlModelColour {
public:
    explicit OpenGlModelColour(
        const GLfloat diffuse[4],
        const GLfloat ambient[4],
        const GLfloat specular[4],
        const GLfloat emissive[4],
        GLfloat shininess
    );

    void apply() const;
private:
    GLfloat diffuse[4];
    GLfloat ambient[4];
    GLfloat specular[4];
    GLfloat emissive[4];
    GLfloat shininess;
};

#endif