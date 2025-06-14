#ifndef OPENGLTRIANGLEMODELDRAWER_H
#define OPENGLTRIANGLEMODELDRAWER_H

#include "../Frustum.h"
#include "OpenGlModelDrawer.h"

class OpenGlTriangleModelDrawer: public OpenGlModelDrawer {
public:
    OpenGlTriangleModelDrawer(const Frustum& frustum);

    void draw(OpenGlTriangleModelBuffer* model_buffer) override;
    int get_triangle_count() const;
private:
    Frustum frustum;
    int triangle_count = 0;

    bool should_draw(const OpenGlTriangleModelBuffer* model_buffer) const;
};

#endif //OPENGLTRIANGLEMODELDRAWER_H
