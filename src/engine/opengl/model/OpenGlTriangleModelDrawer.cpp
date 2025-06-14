#include "OpenGlTriangleModelDrawer.h"

#include "../Frustum.h"

OpenGlTriangleModelDrawer::OpenGlTriangleModelDrawer(const Frustum& frustum) :
    frustum(frustum) {}

bool OpenGlTriangleModelDrawer::should_draw(const OpenGlTriangleModelBuffer* model_buffer) const {
    return frustum.is_sphere_inside(model_buffer->get_bounding_sphere());
}

void OpenGlTriangleModelDrawer::draw(OpenGlTriangleModelBuffer* model_buffer) {
    if (!should_draw(model_buffer)) return;

    glBindVertexArray(model_buffer->vao);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    if (model_buffer->texture != nullptr) {
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        model_buffer->texture->bind();
    }

    if (model_buffer->colour) {
        model_buffer->colour->apply();
    }

    glDrawElements(GL_TRIANGLES, model_buffer->element_count, GL_UNSIGNED_INT, nullptr);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    if (model_buffer->texture != nullptr) {
        model_buffer->texture->unbind();

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
    }

    glBindVertexArray(0);

    triangle_count += model_buffer->element_count / 3;
}

int OpenGlTriangleModelDrawer::get_triangle_count() const {
    return triangle_count;
}