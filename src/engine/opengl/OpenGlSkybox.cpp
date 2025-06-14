#include "OpenGlSkybox.h"
#include <tuple>

#ifndef GLSL
#define GLSL(VERSION, SOURCE) "#version " #VERSION "\n" #SOURCE
#endif

static const char* vert_shader = GLSL(330 core,
    layout (location = 0) in vec3 vert_pos;

    out vec3 tex_coords;
    
    uniform mat4 projection;
    uniform mat4 view;
    
    void main()
    {
        tex_coords = vert_pos;

        mat4 view_no_translation = mat4(mat3(view));
        vec4 pos = projection * view_no_translation * vec4(vert_pos, 1.0);

        gl_Position = pos.xyww;
    }
);

static const char* frag_shader = GLSL(330 core,
    out vec4 frag_color;

    in vec3 tex_coords;
    
    uniform samplerCube skybox_sampler;
    
    void main()
    {
        frag_color = texture(skybox_sampler, tex_coords);
    }
);

OpenGlSkybox::OpenGlSkybox(
    unsigned char* right_data,
    unsigned char* left_data,
    unsigned char* top_data,
    unsigned char* bottom_data,
    unsigned char* front_data,
    unsigned char* back_data,
    int width,
    int height,
    int nr_channels
) : 
    shader_program(vert_shader, frag_shader)
{
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    auto faces = std::array<std::tuple<unsigned char*, GLenum>, 6> {
        std::make_tuple(right_data, GL_TEXTURE_CUBE_MAP_POSITIVE_X),
        std::make_tuple(left_data, GL_TEXTURE_CUBE_MAP_NEGATIVE_X),
        std::make_tuple(top_data, GL_TEXTURE_CUBE_MAP_POSITIVE_Y),
        std::make_tuple(bottom_data, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y),
        std::make_tuple(front_data, GL_TEXTURE_CUBE_MAP_POSITIVE_Z),
        std::make_tuple(back_data, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
    };

    GLenum format =
        nr_channels == 1 ? GL_RED :
        nr_channels == 3 ? GL_RGB :
        nr_channels == 4 ? GL_RGBA :
        GL_RGB;

    for (auto face : faces) {
        glTexImage2D(
            std::get<1>(face),
            0,
            format,
            width,
            height,
            0, format,
            GL_UNSIGNED_BYTE,
            std::get<0>(face)
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    float vertices[] = {  
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
    
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
    
         1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
    
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
    
        -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
    
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    
    glBindVertexArray(0);
}

void OpenGlSkybox::draw(const Mat4x4& view_matrix, const Mat4x4& projection_matrix) const {
    glDepthFunc(GL_LEQUAL);

    shader_program.use();

    shader_program.set_uniform_mat4("view", view_matrix);
    shader_program.set_uniform_mat4("projection", projection_matrix);
    shader_program.set_uniform_1i("skybox_sampler", 0);

    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);

    shader_program.unuse();
}