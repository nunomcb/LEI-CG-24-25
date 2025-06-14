#ifndef OPENGLSKYBOX_H
#define OPENGLSKYBOX_H

#include <GL/glew.h>
#include "OpenGlShaderProgram.h"

class OpenGlSkybox {
public:
    explicit OpenGlSkybox(
        unsigned char* right_data,
        unsigned char* left_data,
        unsigned char* top_data,
        unsigned char* bottom_data,
        unsigned char* front_data,
        unsigned char* back_data,
        int width,
        int height,
        int nr_channels
    );

    void draw(const Mat4x4& view_matrix, const Mat4x4& projection_matrix) const;

private:
    GLuint texture_id, vao, vbo;
    const OpenGlShaderProgram shader_program;
};

#endif