#ifndef CG_MODELBUILDER_H
#define CG_MODELBUILDER_H

#include <map>
#include <array>
#include <vector>
#include "model/Model.h"
#include "model/Vertex.h"

using VertexAttributesKey = std::pair<Vector, Vector>;

class ModelBuilder {
public:
    ModelBuilder() = default;

    void push(const std::array<Vertex, 3>& triangle);

    [[nodiscard]] Model build();
private:
    unsigned int add_or_get_vertex_index(const Vertex& v);

    std::vector<Vertex> unique_vertices;
    std::vector<unsigned int> indices;
    std::map<Vertex, unsigned int> vertex_map;
};

#endif
