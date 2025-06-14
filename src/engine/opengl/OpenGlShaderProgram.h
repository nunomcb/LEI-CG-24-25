#ifndef OPENGLSHADERPROGRAM_H
#define OPENGLSHADERPROGRAM_H

#include <GL/glew.h>
#include <string>

#include "Mat4x4.h"

class OpenGlShaderProgram {
public:
    OpenGlShaderProgram(const char* vert_shader_code, const char* frag_shader_code);

    ~OpenGlShaderProgram();
    
    void use() const;
    void unuse() const;

    void set_uniform_mat4(const std::string& name, const Mat4x4& matrix) const;
    void set_uniform_1i(const std::string& name, int value) const;
private:
    GLuint id;
};

#endif