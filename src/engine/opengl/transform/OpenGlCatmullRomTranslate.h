#ifndef CG_OPENGLCATMULLROMTRANSLATE_H
#define CG_OPENGLCATMULLROMTRANSLATE_H

#include "OpenGlTransform.h"
#include "model/Point.h"
#include "model/Vector.h"
#include <vector>

class OpenGlCatmullRomTranslate : public OpenGlTransform {
public:
    OpenGlCatmullRomTranslate(const std::vector<Point>& points, float time_s, bool align, bool show_curve);
    void apply() override;
    void toggle_curve();

private:
    std::vector<Point> control_points;
    float total_loop_time_s;
    bool align_object;
    bool show_curve;

    std::tuple<Point, Vector> get_point(float t, int *indices);
    std::tuple<Point, Vector> get_global_point(float global_time);
    void render_curve();
    void align(Vector der_arg);
};

#endif // CG_OPENGLCATMULLROMTRANSLATE_H