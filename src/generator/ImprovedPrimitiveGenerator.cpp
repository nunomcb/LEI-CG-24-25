#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <array>

#include "ImprovedPrimitiveGenerator.h"

#include "model/ModelBuilder.h"
#include "model/Point.h"
#include "model/TextureCoord.h"
#include "model/Vector.h"
#include "model/Vertex.h"

static float round(float value, float precision_factor = 100000.0f) {
    return std::round(value * precision_factor) / precision_factor;
}

Model ImprovedPrimitiveGenerator::plane(float length, int divisions) {
    ModelBuilder builder;
    const float div_f = static_cast<float>(divisions);
    const float increment = length / div_f;
    const float start = -length / 2.0f;

    const Vector normal = {0.0f, 1.0f, 0.0f};

    for (int i = 0; i < divisions; ++i) {
        const float current_x = start + static_cast<float>(i) * increment;

        for (int j = 0; j < divisions; ++j) {
            const float current_z = start + static_cast<float>(j) * increment;

            const float u0 = static_cast<float>(i) / div_f;
            const float u1 = static_cast<float>(i + 1) / div_f;
            const float v0 = static_cast<float>(j) / div_f;
            const float v1 = static_cast<float>(j + 1) / div_f;

            const Vertex v_bl({current_x, 0.0f, current_z}, normal, {u0, v0});
            const Vertex v_br({current_x + increment, 0.0f, current_z}, normal, {u1, v0});
            const Vertex v_tl({current_x, 0.0f, current_z + increment}, normal, {u0, v1});
            const Vertex v_tr({current_x + increment, 0.0f, current_z + increment}, normal, {u1, v1});

            builder.push({v_bl, v_tr, v_br});
            builder.push({v_bl, v_tl, v_tr});
        }
    }

    return builder.build();
}

Model ImprovedPrimitiveGenerator::box(float length, int divisions) {
    ModelBuilder builder;
    const float div_f = static_cast<float>(divisions);
    const float increment = length / div_f;
    const float half_len = length / 2.0f;

    const float tex_width = 1.0f / 4.0f;
    const float tex_height = 1.0f / 3.0f;

    auto generate_face = [&](
        const Point& origin,
        const Vector& u_dir,
        const Vector& v_dir,
        const Vector& normal,
        const TextureCoord& tex_origin
    ) {
        float tex_step = 1.0f / static_cast<float>(divisions);

        for (int i = 0; i < divisions; ++i) {
            Vector u_displacement = u_dir * (static_cast<float>(i) * increment);
            float tex_base_u0 = static_cast<float>(i) * tex_step;
            float tex_base_u1 = static_cast<float>(i + 1) * tex_step;

            for (int j = 0; j < divisions; ++j) {
                Vector v_displacement = v_dir * (static_cast<float>(j) * increment);
                float tex_base_v0 = static_cast<float>(j) * tex_step;
                float tex_base_v1 = static_cast<float>(j + 1) * tex_step;

                Point p_bl = origin + u_displacement + v_displacement;
                Point p_br = p_bl + (u_dir * increment);
                Point p_tl = p_bl + (v_dir * increment);
                Point p_tr = p_br + (v_dir * increment);

                Vertex v_bl(p_bl, normal, {tex_origin.u + tex_base_u0 * tex_width, tex_origin.v + tex_base_v0 * tex_height});
                Vertex v_br(p_br, normal, {tex_origin.u + tex_base_u1 * tex_width, tex_origin.v + tex_base_v0 * tex_height});
                Vertex v_tl(p_tl, normal, {tex_origin.u + tex_base_u0 * tex_width, tex_origin.v + tex_base_v1 * tex_height});
                Vertex v_tr(p_tr, normal, {tex_origin.u + tex_base_u1 * tex_width, tex_origin.v + tex_base_v1 * tex_height});

                builder.push({v_bl, v_br, v_tr});
                builder.push({v_bl, v_tr, v_tl});
            }
        }
    };

    generate_face(
        {-half_len, -half_len, half_len},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f * tex_width, 1.0f * tex_height}
    );

    generate_face(
        {half_len, -half_len, -half_len},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, -1.0f},
        {3.0f * tex_width, 1.0f * tex_height}
    );

    generate_face(
        {half_len, -half_len, half_len},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {2.0f * tex_width, 1.0f * tex_height}
    );

    generate_face(
        {-half_len, -half_len, -half_len},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f * tex_width, 1.0f * tex_height}
    );

    generate_face(
        {-half_len, half_len, half_len},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f * tex_width, 2.0f * tex_height}
    );

    generate_face(
        {-half_len, -half_len, -half_len},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, -1.0f, 0.0f},
        {1.0f * tex_width, 0.0f * tex_height}
    );

    return builder.build();
}

Vertex ImprovedPrimitiveGenerator::calculate_sphere_vertex(int i, int j, float radius, int slices, int stacks) {
    const float slices_f = static_cast<float>(slices);
    const float stacks_f = static_cast<float>(stacks);

    const float phi = M_PI * static_cast<float>(i) / stacks_f;
    const float theta = 2.0f * M_PI * static_cast<float>(j) / slices_f;

    const float stack_radius = radius * std::sin(phi);
    const Point pos = {
        stack_radius * std::cos(theta),
        radius * std::cos(phi),
        stack_radius * std::sin(theta)
    };

    const Vector normal = pos.to_vector().normalise();

    float v = 1.0f - (static_cast<float>(i) / stacks_f);
    float u = 1.0f - (static_cast<float>(j) / slices_f);

    const TextureCoord tex_coord = {u, v};

    return {pos, normal, tex_coord};
}

Model ImprovedPrimitiveGenerator::sphere(float radius, int slices, int stacks) {
    ModelBuilder builder;

    Vertex v_pole_n = calculate_sphere_vertex(0, 0, radius, slices, stacks);
    for (int j = 0; j < slices; ++j) {
        Vertex v1 = calculate_sphere_vertex(1, j, radius, slices, stacks);
        Vertex v2 = calculate_sphere_vertex(1, j + 1, radius, slices, stacks);

        builder.push({v_pole_n, v2, v1});
    }

    for (int i = 1; i < stacks - 1; ++i) {
        for (int j = 0; j < slices; ++j) {
            Vertex v_tl = calculate_sphere_vertex(i, j, radius, slices, stacks);
            Vertex v_tr = calculate_sphere_vertex(i, j + 1, radius, slices, stacks);
            Vertex v_bl = calculate_sphere_vertex(i + 1, j, radius, slices, stacks);
            Vertex v_br = calculate_sphere_vertex(i + 1, j + 1, radius, slices, stacks);

            builder.push({v_bl, v_tr, v_br});
            builder.push({v_bl, v_tl, v_tr});
        }
    }

    Vertex v_pole_s = calculate_sphere_vertex(stacks, 0, radius, slices, stacks);
    int last_ring_idx = stacks - 1;
    for (int j = 0; j < slices; ++j) {
        Vertex v1 = calculate_sphere_vertex(last_ring_idx, j, radius, slices, stacks);
        Vertex v2 = calculate_sphere_vertex(last_ring_idx, j + 1, radius, slices, stacks);

        builder.push({v_pole_s, v1, v2});
    }

    return builder.build();
}

Vertex ImprovedPrimitiveGenerator::calculate_cone_vertex(
    int i, int j, float radius, float height, int slices, int stacks
) {
    const float slices_f = static_cast<float>(slices);
    const float stacks_f = static_cast<float>(stacks);
    const float pi = M_PI;

    float t = static_cast<float>(i) / stacks_f;

    float current_height = t * height;
    float current_radius = (1.0f - t) * radius;

    float theta = 2.0f * pi * static_cast<float>(j) / slices_f;

    Point pos = {
        current_radius * std::cos(theta),
        current_height,
        current_radius * std::sin(theta)
    };

    Vector normal = i == stacks
        ? Vector{0.0f, 1.0f, 0.0f}
        : Vector{
            height * std::cos(theta),
            radius,
            height * std::sin(theta)
        }.normalise();

    TextureCoord tex_coord = {static_cast<float>(j) / slices_f, t};

    return Vertex{pos, normal, tex_coord};
}

Model ImprovedPrimitiveGenerator::cone(float radius, float height, int slices, int stacks) {
    ModelBuilder builder;
    const float pi = M_PI;
    const float slices_f = static_cast<float>(slices);

    Point base_center = {0.0f, 0.0f, 0.0f};
    Vector base_normal = {0.0f, -1.0f, 0.0f};
    Vertex v_center(base_center, base_normal);

    for (int j = 0; j < slices; ++j) {
        float theta0 = 2.0f * pi * static_cast<float>(j) / slices_f;
        float theta1 = 2.0f * pi * static_cast<float>(j + 1) / slices_f;

        Point p0 = {radius * std::cos(theta0), 0.0f, radius * std::sin(theta0)};
        Point p1 = {radius * std::cos(theta1), 0.0f, radius * std::sin(theta1)};

        TextureCoord tex0_base = {static_cast<float>(j) / slices_f, static_cast<float>(j) / slices_f};
        TextureCoord tex1_base = {static_cast<float>(j + 1) / slices_f, static_cast<float>(j + 1) / slices};

        Vertex v_curr(p0, base_normal, tex0_base);
        Vertex v_next(p1, base_normal, tex1_base);

        builder.push({v_center, v_curr, v_next});
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            Vertex v_bl = calculate_cone_vertex(i, j, radius, height, slices, stacks);
            Vertex v_br = calculate_cone_vertex(i, j + 1, radius, height, slices, stacks);
            Vertex v_tl = calculate_cone_vertex(i + 1, j, radius, height, slices, stacks);
            Vertex v_tr = calculate_cone_vertex(i + 1, j + 1, radius, height, slices, stacks);

            builder.push({v_bl, v_tl, v_tr});
            builder.push({v_bl, v_tr, v_br});
        }
    }

    return builder.build();
}

Model ImprovedPrimitiveGenerator::bezier_patches(const std::vector<BezierPatch>& patches, int tessellation_level) {
    ModelBuilder builder;
    const float level_f = static_cast<float>(tessellation_level);
    const float step = 1.0f / level_f;

    std::vector<std::vector<Vertex>> grid_vertices(
        tessellation_level + 1,
        std::vector<Vertex>(
            tessellation_level + 1,
            Vertex({0,0,0},{0,0,0})
        )
    );

    for (const auto& patch : patches) {
        for (int i = 0; i <= tessellation_level; ++i) {
            float u = static_cast<float>(i) * step;
            if (i == tessellation_level) u = 1.0f;

            for (int j = 0; j <= tessellation_level; ++j) {
                float v = static_cast<float>(j) * step;
                if (j == tessellation_level) v = 1.0f;

                Point pos = patch.point(u, v);
                Vector norm = patch.normal(u, v);
                TextureCoord tex_coord = {u, v};

                grid_vertices[i][j] = Vertex(pos, norm, tex_coord);
            }
        }

        for (int i = 0; i < tessellation_level; ++i) {
            for (int j = 0; j < tessellation_level; ++j) {
                const Vertex& v_bl = grid_vertices[i][j];
                const Vertex& v_br = grid_vertices[i + 1][j];
                const Vertex& v_tl = grid_vertices[i][j + 1];
                const Vertex& v_tr = grid_vertices[i + 1][j + 1];

                builder.push({v_tl, v_tr, v_br});
                builder.push({v_tl, v_br, v_bl});
            }
        }
    }

    return builder.build();
}
