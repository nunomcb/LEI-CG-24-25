#include "Model.h"

#include <utility>

Model::Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices) :
    vertices(std::move(vertices)),
    indices(std::move(indices)) {}

const Vertex* Model::get_vertices() const {
    return vertices.data();
}

unsigned int Model::get_vertex_count() const {
    return vertices.size();
}

const unsigned int* Model::get_indices() const {
    return indices.data();
}

unsigned int Model::get_index_count() const {
    return indices.size();
}