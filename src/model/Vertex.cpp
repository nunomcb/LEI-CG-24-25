#include "Vertex.h"

Vertex::Vertex(Point position, Vector normal, TextureCoord texture_coord) :
    position(position),
    normal(normal.normalise()),
    texture_coord(texture_coord),
    has_texture_coord(true) {}

Vertex::Vertex(Point position, Vector normal) :
    position(position),
    normal(normal.normalise()),
    texture_coord({0.0f, 0.0f}),
    has_texture_coord(false) {}

Vertex::Vertex() :
    position(Point{0, 0, 0}),
    normal({0, 0, 0}),
    texture_coord({0, 0}),
    has_texture_coord(false) {}

bool Vertex::operator<(const Vertex& other) const {
    if (position < other.position) return true;
    if (other.position < position) return false;
    if (normal < other.normal) return true;
    if (other.normal < normal) return false;

    return texture_coord < other.texture_coord;
}
