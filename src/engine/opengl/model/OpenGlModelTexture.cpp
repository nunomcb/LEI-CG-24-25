#include "OpenGlModelTexture.h"
#include <iostream>

OpenGlModelTexture::OpenGlModelTexture(
    unsigned char* data,
    int width,
    int height,
    int nr_channels
) :
    texture_id(0)
{    
    if (!data) {
        throw std::invalid_argument("data is null");
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint previous_unpack_alignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &previous_unpack_alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLenum format =
        nr_channels == 1 ? GL_RED :
        nr_channels == 3 ? GL_RGB :
        nr_channels == 4 ? GL_RGBA :
        GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glPixelStorei(GL_UNPACK_ALIGNMENT, previous_unpack_alignment);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGlModelTexture::~OpenGlModelTexture() {
    glDeleteTextures(1, &texture_id);
}

void OpenGlModelTexture::bind() const {
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void OpenGlModelTexture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}