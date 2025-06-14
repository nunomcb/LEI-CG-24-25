#include <GL/glew.h>
#include <vector>

#include "OpenGlLineModelBuffer.h"

OpenGlLineModelBuffer::OpenGlLineModelBuffer(
    Vector v1, Vector v2, Color color
): vertices({v1, v2}), colors({color, color}){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &colorBuffer);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(
        GL_ARRAY_BUFFER, 2 * sizeof(Vector), vertices.data(), GL_STATIC_DRAW
    );

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(unsigned int), arrangement, GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(
        GL_ARRAY_BUFFER, 6 * sizeof(float), colors.data(), GL_STATIC_DRAW
    );
    glColorPointer(3, GL_FLOAT, 0, 0);

    glEnableVertexAttribArray(0);
}

void OpenGlLineModelBuffer::set_enabled(bool enabled) {
    this->enabled = enabled;
}

bool OpenGlLineModelBuffer::is_enabled() const {
    return enabled;
}

void OpenGlLineModelBuffer::draw() {
    if (!enabled) {
        return;
    }

    glBindVertexArray(vao);

    GLfloat mat_emission[] = { colors[0].red, colors[0].green, colors[0].blue, 1.0f };
    GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);

    glEnableClientState(GL_COLOR_ARRAY);

    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr);

    glDisableClientState(GL_COLOR_ARRAY);
    glBindVertexArray(0);
}
