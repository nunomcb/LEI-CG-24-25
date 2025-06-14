#ifndef CG_OPENGLWINDOW_H
#define CG_OPENGLWINDOW_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include "OpenGlScene.h"

class OpenGlWindow {
public:
    OpenGlWindow(
        std::string title,
        int height,
        int width,
        void (*on_key_press)(unsigned char, int, int),
        void (*on_special_key_press)(int, int, int)
    );

    const std::vector<config::Light>* lights_config;
    const std::string window_title;
    OpenGlScene* scene;
    int width, height;

    void init();
    void start(OpenGlScene* scene);
};

#endif //CG_OPENGLWINDOW_H
