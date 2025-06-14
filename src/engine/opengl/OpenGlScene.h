#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

#include "Frustum.h"
#include "OpenGlCamera.h"
#include "OpenGlGroup.h"
#include "OpenGlSkybox.h"
#include "light/OpenGlLight.h"

class OpenGlScene {
public:
    OpenGlScene(
        OpenGlCamera* camera,
        OpenGlGroup* group,
        std::vector<OpenGlLight*> lights,
        OpenGlSkybox* skybox = nullptr
    );

    GLenum mode = GL_FILL;

    void initialize_lights() const;
    void setup_projection_matrix(int width, int height) const;
    void update_active_lights() const;

    int draw() const;
    void set_mode(GLenum mode);

    bool is_show_normals_enabled() const;
    void set_show_normals_enabled(bool show_normals);

private:
    OpenGlCamera* camera;
    OpenGlGroup* group;
    OpenGlSkybox* skybox;
    std::vector<OpenGlLight*> lights;
    bool show_normals = false;
};



#endif //OPENGLSCENE_H
