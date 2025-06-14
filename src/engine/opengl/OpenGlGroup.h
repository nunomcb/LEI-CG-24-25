#ifndef CG_OPENGLGROUP_H
#define CG_OPENGLGROUP_H

#include <vector>

#include "model/OpenGlTriangleModelBuffer.h"
#include "transform/OpenGlTransform.h"
#include "model/OpenGlTriangleModelDrawer.h"

class OpenGlGroup {
public:
    OpenGlGroup(
        const std::vector<OpenGlModelBuffer*>& models,
        const std::vector<OpenGlTransform*>& transforms,
        const std::vector<OpenGlGroup*>& child_groups
    );

    void draw(OpenGlModelDrawer* drawer);

private:
    const std::vector<OpenGlModelBuffer*> models;
    const std::vector<OpenGlTransform*> transforms;
    const std::vector<OpenGlGroup*> child_groups;
};


#endif //CG_OPENGLGROUP_H
