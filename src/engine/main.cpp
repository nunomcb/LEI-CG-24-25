#include <filesystem>
#include <tinyxml/tinyxml2.h>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <queue>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <model/serialise/ObjSerialiser.h>
#include "opengl/OpenGlWindow.h"

#include "config.h"
#include "opengl/OpenGlGroup.h"
#include "opengl/light/OpenGlLight.h"
#include "opengl/light/OpenGlPointLight.h"
#include "opengl/light/OpenGlDirectionalLight.h"
#include "opengl/light/OpenGlSpotLight.h"
#include "opengl/model/OpenGlModelColour.h"
#include "opengl/model/OpenGlModelTexture.h"
#include "opengl/model/OpenGlTriangleModelBuffer.h"
#include "opengl/transform/OpenGlScale.h"
#include "opengl/transform/OpenGlRotate.h"
#include "opengl/transform/OpenGlTranslate.h"
#include "opengl/transform/OpenGlCatmullRomTranslate.h"
#include "opengl/model/OpenGlLineModelBuffer.h"
#include "opengl/OpenGlScene.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77

static OpenGlScene* scene;
static OpenGlCamera* camera;
static OpenGlSkybox* skybox;
static std::array<OpenGlLineModelBuffer*, 3> axes;
static std::vector<OpenGlTriangleModelBuffer*> all_models;
static std::vector<OpenGlCatmullRomTranslate*> catmull_rom_translates;

void on_special_key_press_callback(int key, int x, int y) {

    switch (key) {
    }
}

void on_key_press_callback(unsigned char key, int x, int y) {
    switch (key) {
        case '+': camera->zoom_in(0.1); break;
        case '-': camera->zoom_out(0.1); break;
        case 'w': camera->move_forward(1); break;
        case 's': camera->move_back(1); break;
        case 'a': camera->move_left(1); break;
        case 'd': camera->move_right(1); break;
        case 'W': camera->look_up(0.1f); break;
        case 'S': camera->look_down(0.1f); break;
        case 'A': camera->look_left(0.1f); break;
        case 'D': camera->look_right(0.1f); break;
        case 'c': for (auto translate : catmull_rom_translates) translate->toggle_curve(); break;
        case 'm': scene->set_mode(scene->mode == GL_FILL ? GL_LINE : GL_FILL); break;
        case 'l': if (glIsEnabled(GL_LIGHTING)) glDisable(GL_LIGHTING); else glEnable(GL_LIGHTING); break;
        case '1': if (glIsEnabled(GL_LIGHT0)) glDisable(GL_LIGHT0); else glEnable(GL_LIGHT0); break;
        case '2': if (glIsEnabled(GL_LIGHT1)) glDisable(GL_LIGHT1); else glEnable(GL_LIGHT1); break;
        case '3': if (glIsEnabled(GL_LIGHT2)) glDisable(GL_LIGHT2); else glEnable(GL_LIGHT2); break;
        case '4': if (glIsEnabled(GL_LIGHT3)) glDisable(GL_LIGHT3); else glEnable(GL_LIGHT3); break;
        case '5': if (glIsEnabled(GL_LIGHT4)) glDisable(GL_LIGHT4); else glEnable(GL_LIGHT4); break;
        case '6': if (glIsEnabled(GL_LIGHT5)) glDisable(GL_LIGHT5); else glEnable(GL_LIGHT5); break;
        case '7': if (glIsEnabled(GL_LIGHT6)) glDisable(GL_LIGHT6); else glEnable(GL_LIGHT6); break;
        case '8': if (glIsEnabled(GL_LIGHT7)) glDisable(GL_LIGHT7); else glEnable(GL_LIGHT7); break;
        case '/': for (auto axis : axes) axis->set_enabled(!axis->is_enabled()); break;
        case 'n': for (auto model : all_models) scene->set_show_normals_enabled(!scene->is_show_normals_enabled()); break;
    }
}

void print_controls() {
    auto commands =
        "[w] move forward\n"
        "[s] move backward\n"
        "[a] move left\n"
        "[d] move right\n"
        "[W] look up\n"
        "[S] look down\n"
        "[A] look left\n"
        "[D] look right\n"
        "[+] zoom in\n"
        "[-] zoom out\n"
        "[m] change mode (GL_FILL, GL_LINE)\n"
        "[c] show Catmull-Rom curves\n"
        "[/] show axes\n"
        "[n] show normals\n"
        "[l] toggle lighting\n"
        "[1-8] toggle lights 0-7, respectively\n";

    std::cout << commands << std::endl;
}

OpenGlGroup* build_axes(OpenGlGroup* group) {
    auto x = new OpenGlLineModelBuffer(
        Vector({-50.0f, 0.0f, 0.0f}),
        Vector({50.0f, 0.0f, 0.0f}),
        Color({1.0f, 0.0f, 0.0f})
    );

    auto y = new OpenGlLineModelBuffer(
        Vector({0.0f, -50.0f, 0.0f}),
        Vector({0.0f, 50.0f, 0.0f}),
        Color({0.0f, 1.0f, 0.0f})
    );

    auto z = new OpenGlLineModelBuffer(
        Vector({0.0f, 0.0f, -50.0f}),
        Vector({0.0f, 0.0f, 50.0f}),
        Color({0.0f, 0.0f, 1.0f})
    );

    x->set_enabled(false);
    y->set_enabled(false);
    z->set_enabled(false);

    axes = {x, y, z};

    return new OpenGlGroup({x, y, z}, {}, {group});
}

static std::array<GLfloat, 4> config_colour_to_glfloat_array(const config::Colour& cfg_colour, GLfloat alpha = 1.0f) {
    return {
        static_cast<GLfloat>(cfg_colour.r) / 255.0f,
        static_cast<GLfloat>(cfg_colour.g) / 255.0f,
        static_cast<GLfloat>(cfg_colour.b) / 255.0f,
        alpha
    };
}

static std::vector<OpenGlTransform*> build_transforms_for_group(
    const std::vector<config::Transform>& transform_configs
) {
    std::vector<OpenGlTransform*> transforms;

    for (const auto& transform_config : transform_configs) {
        if (transform_config.is_rotate) {
            auto config = transform_config.rotate;

            if (config->time != 0) {
                transforms.push_back(
                    new OpenGlRotate(config->time, config->x, config->y, config->z)
                );
            }
            else {
                transforms.push_back(
                    new OpenGlRotate(config->angle, config->x, config->y, config->z)
                );
            }
        }
        else if (transform_config.is_scale) {
            auto config = transform_config.scale;

            transforms.push_back(
                new OpenGlScale(config->x, config->y, config->z)
            );
        }
        else if (transform_config.is_translate) {
            auto config = transform_config.translate;

            transforms.push_back(
                new OpenGlTranslate(config->x, config->y, config->z)
            );
        }
        else if (transform_config.is_cr_translate) {
            auto config = transform_config.cr_translate;

            auto transform = new OpenGlCatmullRomTranslate(
                config->points, config->time, config->align, false
            );

            catmull_rom_translates.push_back(transform);

            transforms.push_back(transform);
        }
    }

    return transforms;
}

static std::vector<OpenGlModelBuffer*> build_models_for_group(
    const std::string& base_path,
    const std::vector<config::Model>& model_configs
) {
    std::vector<OpenGlModelBuffer*> models;

    for (const auto& model_config : model_configs) {
        std::ifstream model_file (base_path + "/" + model_config.file);

        if (!model_file.good()) {
            throw std::invalid_argument(
                ("Couldn't open model file " + model_config.file + " for reading").c_str()
            );
        }

        const auto model_colour = new OpenGlModelColour(
            config_colour_to_glfloat_array(model_config.colour.diffuse).data(),
            config_colour_to_glfloat_array(model_config.colour.ambient).data(),
            config_colour_to_glfloat_array(model_config.colour.specular).data(),
            config_colour_to_glfloat_array(model_config.colour.emissive).data(),
            model_config.colour.shininess        
        );

        OpenGlModelTexture* model_texture = nullptr;
        if (model_config.texture_file.has_value()) {
            std::string texture_path_str = base_path + "/" + model_config.texture_file.value();
            
            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true);
            unsigned char *data = stbi_load(texture_path_str.c_str(), &width, &height, &nrChannels, 0);

            model_texture = new OpenGlModelTexture(data, width, height, nrChannels);

            stbi_image_free(data);
        }

        auto model = new OpenGlTriangleModelBuffer(
            ObjSerialiser().deserialise(model_file),
            model_colour,
            model_texture
        );
        
        all_models.emplace_back(model);
        models.emplace_back(model);

        model_file.close();
    }

    return models;
}

static std::vector<OpenGlLight*> build_lights(const std::vector<config::Light>& light_configs) {
    std::vector<OpenGlLight*> opengl_lights;

    auto light_ids = std::queue<GLenum>({
        GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3,
        GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7,
    });

    for (const auto& this_light : light_configs) {
        auto light_id = light_ids.front();

        light_ids.pop();

        if (this_light.is_point) {
            auto* conf = this_light.point;

            opengl_lights.push_back(new OpenGlPointLight(
                light_id,
                conf->pos_x * 1.0f, conf->pos_y * 1.0f, conf->pos_z * 1.0f,
                conf->r, conf->g, conf->b
            ));
        }
        else if (this_light.is_directional) {
            auto* conf = this_light.directional;

            opengl_lights.push_back(new OpenGlDirectionalLight(
                light_id,
                conf->dir_x * 1.0f, conf->dir_y * 1.0f, conf->dir_z * 1.0f,
                conf->r, conf->g, conf->b
            ));
        }
        else if (this_light.is_spotlight) {
            auto* conf = this_light.spotlight;

            opengl_lights.push_back(new OpenGlSpotLight(
                light_id,
                conf->pos_x * 1.0f, conf->pos_y * 1.0f, conf->pos_z * 1.0f,
                conf->dir_x * 1.0f, conf->dir_y * 1.0f, conf->dir_z * 1.0f,
                conf->r, conf->g, conf->b,
                conf->cutoff,
                conf->exponent
            ));
        }
    }

    return opengl_lights;
}

OpenGlGroup* build_groups(const std::string& base_path, const config::Group& group_config) {
    std::vector<OpenGlGroup*> child_groups;

    std::vector<OpenGlModelBuffer*> models = build_models_for_group(base_path, group_config.models);
    std::vector<OpenGlTransform*> transforms = build_transforms_for_group(group_config.transforms);

    for (const auto& child_group_config : group_config.groups) {
        child_groups.push_back(build_groups(base_path, child_group_config));
    }

    return new OpenGlGroup(models, transforms, child_groups);
}

static config::EngineConfig load_config(char* file_name) {
    const auto basedir = std::filesystem::path{file_name}.parent_path().string();

    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(file_name) != tinyxml2::XML_SUCCESS) {
        throw std::invalid_argument(doc.ErrorStr());
    }

    return config::EngineConfig(basedir, &doc);
}

static int main_main(int argc, char **argv) {
    auto engine_config = load_config(argv[1]);
    auto window_config = engine_config.world.window;
    auto camera_config = engine_config.world.camera;
    auto group_config = engine_config.world.group;

    auto window = OpenGlWindow("CG", window_config.height, window_config.width, on_key_press_callback, on_special_key_press_callback);

    if (engine_config.world.skybox.has_value()) {
        const auto& skybox_config = engine_config.world.skybox.value();

        std::vector skybox_face_paths = {
            engine_config.base_path + "/" + skybox_config.right,	
            engine_config.base_path + "/" + skybox_config.left,
            engine_config.base_path + "/" + skybox_config.top,
            engine_config.base_path + "/" + skybox_config.bottom,
            engine_config.base_path + "/" + skybox_config.front,
            engine_config.base_path + "/" + skybox_config.back
        };

        std::array<unsigned char*, 6> face_data;
        int width, height, nrChannels;

        for (int i = 0; i < 6; i++) {
            face_data[i] = stbi_load(skybox_face_paths[i].c_str(), &width, &height, &nrChannels, 0);
        }

        skybox = new OpenGlSkybox(
            face_data[0],
            face_data[1],
            face_data[2],
            face_data[3],
            face_data[4],
            face_data[5],
            width,
            height,
            nrChannels
        );
    }

    camera = new OpenGlCamera(camera_config);

    auto lights = build_lights(engine_config.world.lights);
    auto group = build_axes(build_groups(engine_config.base_path, engine_config.world.group));

    scene = new OpenGlScene(camera, group, lights, skybox);

    print_controls();

    window.start(scene);

    return 1;
}

int main(int argc, char **argv) {
    try {
        return main_main(argc, argv);
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 1;
}