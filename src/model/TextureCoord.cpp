#include "TextureCoord.h"

bool TextureCoord::operator < (const TextureCoord& other) const {
    return u < other.u
        || u == other.u && v < other.v;
}