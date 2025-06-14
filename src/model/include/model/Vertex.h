#ifndef VERTEX_H
#define VERTEX_H

#include "Point.h"
#include "TextureCoord.h"
#include "Vector.h"

struct Vertex {
public:
    Vertex(Point position, Vector normal, TextureCoord texture_coord);
    Vertex(Point position, Vector normal);
    Vertex();

    Point position;
    Vector normal;
    TextureCoord texture_coord;

    bool has_texture_coord;

    bool operator < (const Vertex& other) const;
};

#endif //VERTEX_H
