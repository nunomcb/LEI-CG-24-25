#ifndef CG_OPENGLTRANSLATE_H
#define CG_OPENGLTRANSLATE_H

#include "OpenGlTransform.h"

class OpenGlTranslate : public OpenGlTransform {
public:
    OpenGlTranslate(float x, float y, float z);

    void apply() override;
private:
    const float x, y, z;
};

#endif //CG_OPENGLTRANSLATE_H
