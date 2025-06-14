#ifndef OPENGLMODELTEXTURE_H
#define OPENGLMODELTEXTURE_H

#include <string>
#include <GL/glew.h>

class OpenGlModelTexture {
public:
    OpenGlModelTexture(unsigned char* data, int width, int height, int nr_channels);

    ~OpenGlModelTexture();

    void bind() const;
    void unbind() const;

private:
    GLuint texture_id;
};

#endif