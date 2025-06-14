#ifndef CG_OPENGLSCALE_H
#define CG_OPENGLSCALE_H

#include "OpenGlTransform.h"

class OpenGlScale : public OpenGlTransform{
public:
    OpenGlScale(float x, float y, float z);

    void apply() override;
private:
    float x, y, z;
};


#endif //CG_OPENGLSCALE_H
