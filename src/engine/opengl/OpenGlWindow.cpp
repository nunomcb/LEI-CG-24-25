#include "OpenGlWindow.h"

#include <stdexcept>
#include <utility>
#include <iostream>

#include "OpenGlCamera.h"

OpenGlWindow* obj;
void (*on_key_press_callback)(unsigned char, int, int);
void (*on_special_key_press_callback)(int, int, int);

static int frame_count = 0;
static int previous_time = 0;
static char fps_string[50] = "FPS: 0";

void render_bitmap_string(float x, float y, void *font, const std::string &str) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, obj->width, 0.0, obj->height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT | GL_CURRENT_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 1.0f, 1.0f);

    glRasterPos2f(x, y);

    for (char c : str) {
        glutBitmapCharacter(font, c);
    }

    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void render() {
    frame_count++;
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    int time_interval = current_time - previous_time;

    if (previous_time == 0) {
        previous_time = current_time;
    }

    if (time_interval > 1000) {
        float fps_val = frame_count / (time_interval / 1000.0f);
        sprintf(fps_string, "FPS: %.2f", fps_val);
        previous_time = current_time;
        frame_count = 0;
    }

    int total_triangles = obj->scene->draw();

    float text_x = 10.0f;
    float text_y_fps = obj->height - 20.0f;
    float text_y_triangles = obj->height - 40.0f;
    render_bitmap_string(text_x, text_y_fps, GLUT_BITMAP_9_BY_15, fps_string);
    render_bitmap_string(text_x, text_y_triangles, GLUT_BITMAP_9_BY_15, "TRIANGLES: " + std::to_string(total_triangles));

    glutSwapBuffers();
}

void change_size(const int width, const int height) {
    obj->width = width > 0 ? width : 1;
    obj->height = height > 0 ? height : 1;

    obj->scene->setup_projection_matrix(obj->width, obj->height);
}

void OpenGlWindow::init() {
    char *dummy_argv [1];
    int dummy_argc = 1;

    dummy_argv[0] = strdup("app");

    obj = this;

    glutInit(&dummy_argc, dummy_argv);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(
        (glutGet(GLUT_SCREEN_WIDTH) - width) / 2,
        (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2
    );

    glutInitWindowSize(width, height);
    glutCreateWindow(window_title.c_str());

    auto glew_out = glewInit();

    if (glew_out != GLEW_OK)
    {
        throw std::runtime_error(reinterpret_cast<const char *>(glewGetErrorString(glew_out)));
    }

    glClearColor (0.0, 0.0, 0.0, 0.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutReshapeFunc(change_size);
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutKeyboardFunc(on_key_press_callback);
    glutSpecialFunc(on_special_key_press_callback);
}

OpenGlWindow::OpenGlWindow(
    std::string window_title,
    int height,
    int width,
    void (*on_key_press)(unsigned char, int, int),
    void (*on_special_key_press)(int, int, int)
) :
    window_title(std::move(window_title)),
    height(height),
    width(width),
    scene(nullptr)
{
    on_key_press_callback = on_key_press;
    on_special_key_press_callback = on_special_key_press;

    init();
}

void OpenGlWindow::start(OpenGlScene* scene) {
    this->scene = scene;

    scene->initialize_lights();

    glutMainLoop();
}