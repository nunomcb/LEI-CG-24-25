#ifndef CG_OPENGLTRIANGLEMODELBUFFER_H
#define CG_OPENGLTRIANGLEMODELBUFFER_H

#include <GL/glew.h>
#include <model/Model.h>

#include "../BoundingSphere.h"
#include "OpenGlModelBuffer.h"
#include "OpenGlModelColour.h"
#include "OpenGlModelTexture.h"

class OpenGlTriangleModelBuffer: public OpenGlModelBuffer {
public:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLsizei element_count = 0;

    const OpenGlModelColour* colour;
    const OpenGlModelTexture* texture;

    std::vector<Vertex> vertices;

    explicit OpenGlTriangleModelBuffer(
        const Model& model,
        const OpenGlModelColour* colour,
        const OpenGlModelTexture* texture = nullptr
    );

    const BoundingSphere& get_bounding_sphere() const;
    const std::vector<Vertex>& get_vertices() const;

    void draw() override;
private:
    BoundingSphere bounding_sphere;
};

#endif //CG_OPENGLTRIANGLEMODELBUFFER_H
