#ifndef OPENGLMODELDRAWER_H
#define OPENGLMODELDRAWER_H

#include "OpenGlTriangleModelBuffer.h"

class OpenGlModelDrawer {
public:
    virtual void draw(OpenGlTriangleModelBuffer* model_buffer) = 0;;
};

#endif //OPENGLMODELDRAWER_H
