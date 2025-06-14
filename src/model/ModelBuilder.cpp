#include "ModelBuilder.h"

#include <utility>
#include <cmath>

static float round(float value, float precision) {
    return std::round(value * precision) / precision;
}

static Vertex round(Vertex v, float precision = 100000.0f) {
    return {
        {round(v.position.x, precision), round(v.position.y, precision), round(v.position.z, precision)},
        Vector{round(v.normal.x, precision), round(v.normal.y, precision), round(v.normal.z, precision)}.normalise(),
        {round(v.texture_coord.u, precision), round(v.texture_coord.v, precision)}
    };
}

void ModelBuilder::push(const std::array<Vertex, 3>& triangle_vertices) {
    auto rounded_vertices = std::array<Vertex, 3>();

    for (int i = 0; i < 3; ++i) {
        rounded_vertices[i] = round(triangle_vertices[i]);
    }

    for (auto current_vertex : rounded_vertices) {
        auto maybe_found_vertex = vertex_map.find(current_vertex);
        unsigned int index;

        if (maybe_found_vertex == vertex_map.end()) {
            index = static_cast<unsigned int>(unique_vertices.size());

            unique_vertices.push_back(current_vertex);
            vertex_map[current_vertex] = index;
        } else {
            index = maybe_found_vertex->second;
        }

        indices.push_back(index);
    }
}

Model ModelBuilder::build() {
    return {std::move(unique_vertices), std::move(indices)};
}
