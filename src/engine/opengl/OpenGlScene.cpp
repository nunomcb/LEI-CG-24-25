#include "OpenGlScene.h"

#include <GL/glut.h>

#include "model/OpenGlTriangleModelDrawer.h"
#include "model/OpenGlTriangleModelNormalDrawer.h"

OpenGlScene::OpenGlScene(
    OpenGlCamera* camera, OpenGlGroup* group, std::vector<OpenGlLight*> lights, OpenGlSkybox* skybox
):
    camera(camera),
    group(group),
    lights(lights),
    skybox(skybox) {}

void OpenGlScene::set_mode(GLenum mode) {
    this->mode = mode;
}

bool OpenGlScene::is_show_normals_enabled() const {
    return show_normals;
}

void OpenGlScene::set_show_normals_enabled(bool show_normals_enabled) {
    show_normals = show_normals_enabled;
}


void OpenGlScene::initialize_lights() const {
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    GLfloat global_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    for (size_t i = 0; i < lights.size() && i < GL_MAX_LIGHTS; ++i) {
        GLenum light_id = GL_LIGHT0 + static_cast<GLenum>(i);

        lights[i]->setup();
    }
}

void OpenGlScene::update_active_lights() const {
    for (size_t i = 0; i < lights.size() && i < GL_MAX_LIGHTS; ++i) {
        GLenum light_id = GL_LIGHT0 + static_cast<GLenum>(i);

        lights[i]->update();
    }
}

int OpenGlScene::draw() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, mode);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Mat4x4 proj_matrix = camera->get_projection_matrix();
    Mat4x4 view_matrix = camera->get_view_matrix();
    Mat4x4 inverse_view_matrix = camera->get_inverse_view_matrix();

    auto current_view_matrix = camera->get_view_matrix();

    glLoadMatrixf(current_view_matrix.data); 

    if (skybox != nullptr) {
        skybox->draw(camera->get_view_matrix(), camera->get_projection_matrix());
    }

    update_active_lights();

    glPushMatrix();

    auto view_frustum = Frustum(view_matrix, proj_matrix, inverse_view_matrix);

    OpenGlTriangleModelDrawer drawer(view_frustum);

    group->draw(&drawer);

    if (show_normals) {
        OpenGlTriangleModelNormalDrawer normal_drawer(view_frustum);

        group->draw(&normal_drawer);
    }

    glPopMatrix();

    return drawer.get_triangle_count();
}

void OpenGlScene::setup_projection_matrix(int width, int height) const {
    camera->update_projection(width, height);
}
