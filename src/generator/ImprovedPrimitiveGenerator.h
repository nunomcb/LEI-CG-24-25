#ifndef IMPROVEDPRIMITIVEGENERATOR_H
#define IMPROVEDPRIMITIVEGENERATOR_H

#include "model/Model.h"
#include "BezierPatch.h"

class ImprovedPrimitiveGenerator {
public:
    Model plane(float length, int divisions);
    Model box(float length, int divisions);
    Model sphere(float radius, int slices, int stacks);
    Model cone(float radius, float height, int slices, int stacks);
    Model bezier_patches(const std::vector<BezierPatch>& patches, int tessellation_level);
private:
    Vertex calculate_sphere_vertex(int i, int j, float radius, int slices, int stacks);
    Vertex calculate_cone_vertex(int i, int j, float radius, float height, int slices, int stacks);
};



#endif //IMPROVEDPRIMITIVEGENERATOR_H
