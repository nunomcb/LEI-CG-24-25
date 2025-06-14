#ifndef CG_OPENGLLINEMODELBUFFER_H
#define CG_OPENGLLINEMODELBUFFER_H

#include <model/Vector.h>

#include "../../Color.h"
#include "OpenGlModelBuffer.h"

class OpenGlLineModelBuffer: public OpenGlModelBuffer {
public:
    explicit OpenGlLineModelBuffer(
        Vector v1, Vector v2, Color color
    );

    void draw() override;
    void set_enabled(bool enabled);
    bool is_enabled() const;

private:
    std::vector<Vector> vertices;
    std::vector<Color> colors;
    unsigned int arrangement[2] = {0, 1};
    unsigned int vao{}, vbo{}, ebo{}, colorBuffer{};

    bool enabled = true;
};


#endif //CG_OPENGLLINEMODELBUFFER_H
