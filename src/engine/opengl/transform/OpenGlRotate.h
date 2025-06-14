#ifndef CG_OPENGLROTATE_H
#define CG_OPENGLROTATE_H

#include "OpenGlTransform.h"

class OpenGlRotate : public OpenGlTransform {
public:
    OpenGlRotate(float angle, float x, float y, float z);
    OpenGlRotate(int time, float x, float y, float z);

    void apply() override;
private:
    int time;
    float angle, x, y, z;

    bool is_timed() const;
};

#endif
