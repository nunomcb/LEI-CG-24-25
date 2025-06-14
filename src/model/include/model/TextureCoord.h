#ifndef TEXTURECOORD_H
#define TEXTURECOORD_H

struct TextureCoord {
public:
    float u, v;

    bool operator < (const TextureCoord& other) const;
};
#endif //TEXTURECOORD_H
