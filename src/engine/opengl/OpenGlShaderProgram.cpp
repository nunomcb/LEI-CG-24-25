#include "OpenGlShaderProgram.h"
#include <stdexcept>

OpenGlShaderProgram::OpenGlShaderProgram(const char* vert_shader_code, const char* frag_shader_code) {
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLint success;
    GLchar log[1024];

    glShaderSource(vertex_shader, 1, &vert_shader_code, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 1024, NULL, log);

        throw std::runtime_error(log);
    }
    
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragment_shader, 1, &frag_shader_code, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment_shader, 1024, NULL, log);

        throw std::runtime_error(log);
    }

    id = glCreateProgram();

    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(id, 1024, NULL, log);

        throw std::runtime_error(log);
    }
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

OpenGlShaderProgram::~OpenGlShaderProgram() {
    glDeleteProgram(id);
}

void OpenGlShaderProgram::use() const {
    glUseProgram(id);
}

void OpenGlShaderProgram::unuse() const {
    glUseProgram(0);
}

void OpenGlShaderProgram::set_uniform_1i(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void OpenGlShaderProgram::set_uniform_mat4(const std::string& name, const Mat4x4& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, matrix.data);
}
