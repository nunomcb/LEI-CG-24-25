#ifndef CG_MODEL_H
#define CG_MODEL_H

#include <vector>

#include "model/Vertex.h"

class Model {
public:
    Model(
        std::vector<Vertex> vertices,
        std::vector<unsigned int> indices
    );

    [[nodiscard]] const Vertex* get_vertices() const;
    [[nodiscard]] unsigned int get_vertex_count() const;
    [[nodiscard]] const unsigned int* get_indices() const;
    [[nodiscard]] unsigned int get_index_count() const;

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

#endif
