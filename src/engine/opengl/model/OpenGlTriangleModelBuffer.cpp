#include "OpenGlTriangleModelBuffer.h"

#include <cmath>
#include <algorithm>

OpenGlTriangleModelBuffer::OpenGlTriangleModelBuffer(
    const Model& model,
    const OpenGlModelColour* colour,
    const OpenGlModelTexture* texture
) :
    colour(colour),
    texture(texture),
    vao(0),
    vbo(0),
    ebo(0)
{
    const Vertex* vertices = model.get_vertices();
    const unsigned int* indices = model.get_indices();

    const size_t vertex_count = model.get_vertex_count();
    const size_t index_count = model.get_index_count();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

    if (texture != nullptr) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture_coord));
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    if (texture != nullptr) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    this->vertices.assign(vertices, vertices + vertex_count);
    this->element_count = static_cast<GLsizei>(index_count);

    std::vector<Point> pos(this->vertices.size());
    std::transform(
        this->vertices.begin(),
        this->vertices.end(),
        pos.begin(),
        [](const Vertex& v) { return v.position; }
    );

    bounding_sphere = BoundingSphere(pos);
}

void OpenGlTriangleModelBuffer::draw() {
    // TODO remove
}

const BoundingSphere& OpenGlTriangleModelBuffer::get_bounding_sphere() const {
    return bounding_sphere;
}

const std::vector<Vertex>& OpenGlTriangleModelBuffer::get_vertices() const {
    return vertices;
}
