#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>

#include "OpenGlRotate.h"

OpenGlRotate::OpenGlRotate(float angle, float x, float y, float z) :
    time(0), angle(angle), x(x), y(y), z(z) {}

OpenGlRotate::OpenGlRotate(int time, float x, float y, float z) :
    time(time), angle(0), x(x), y(y), z(z) {}

bool OpenGlRotate::is_timed() const {
    return time > 0;
}

void OpenGlRotate::apply() {
    float actual_angle;

    if (is_timed()) {
        auto elapsed_time_ms = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));

        actual_angle = std::fmod(elapsed_time_ms * 360.0f / (time * 1000.0f), 360.0f);
    }
    else {
        actual_angle = angle;
    }

    glRotatef(actual_angle, x, y, z);
}
