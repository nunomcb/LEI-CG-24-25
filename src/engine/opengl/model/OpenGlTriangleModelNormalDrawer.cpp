#include "OpenGlTriangleModelNormalDrawer.h"

OpenGlTriangleModelNormalDrawer::OpenGlTriangleModelNormalDrawer(const Frustum& frustum) :
    frustum(frustum) {}

bool OpenGlTriangleModelNormalDrawer::should_draw(const OpenGlTriangleModelBuffer* model_buffer) const {
    return frustum.is_sphere_inside(model_buffer->get_bounding_sphere());
}

void OpenGlTriangleModelNormalDrawer::draw(OpenGlTriangleModelBuffer* model_buffer) {
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_LINE_BIT | GL_ENABLE_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.4f, 1.0f, 0.4f);
    glLineWidth(1.0f);

    const float normal_display_scale = 0.5f;

    glBegin(GL_LINES);
    for (const auto& vertex : model_buffer->vertices) {
        glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
        glVertex3f(
            vertex.position.x + vertex.normal.x * normal_display_scale,
            vertex.position.y + vertex.normal.y * normal_display_scale,
            vertex.position.z + vertex.normal.z * normal_display_scale
        );
    }
    glEnd();

    glPopAttrib();
}