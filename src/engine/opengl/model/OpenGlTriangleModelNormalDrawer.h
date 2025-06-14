#ifndef OPENGLTRIANGLEMODELNORMALDRAWER_H
#define OPENGLTRIANGLEMODELNORMALDRAWER_H

#include "OpenGlModelDrawer.h"
#include "../Frustum.h"

class OpenGlTriangleModelNormalDrawer: public OpenGlModelDrawer {
public:
    OpenGlTriangleModelNormalDrawer(const Frustum& frustum);

    void draw(OpenGlTriangleModelBuffer* model_buffer) override;

private:
    const Frustum frustum;

    bool should_draw(const OpenGlTriangleModelBuffer* model_buffer) const;
};

#endif
