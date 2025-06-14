#include <GL/glew.h>

#include "OpenGlGroup.h"

OpenGlGroup::OpenGlGroup(
    const std::vector<OpenGlModelBuffer*>& models,
    const std::vector<OpenGlTransform*>& transforms,
    const std::vector<OpenGlGroup*>& child_groups
) :
    models(models),
    transforms(transforms),
    child_groups(child_groups) {}

void OpenGlGroup::draw(OpenGlModelDrawer* drawer) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    for (auto t : transforms) t->apply();

    for (OpenGlModelBuffer* model_buffer_ptr : models) {
        if (auto* model_buffer = dynamic_cast<OpenGlTriangleModelBuffer*>(model_buffer_ptr)) {
            drawer->draw(model_buffer);
        } else {
            model_buffer_ptr->draw();
        }
    }

    for (auto c : child_groups) {
        c->draw(drawer);
    }

    glPopMatrix();
}
