#include <stdexcept>

#include "config.h"

static const tinyxml2::XMLElement* get_xml_element(const tinyxml2::XMLNode* node, const char* field) {
    if (node == nullptr) {
        throw std::invalid_argument("wut");
    }

    return node->FirstChildElement(field);
}

static float get_float_attr(const tinyxml2::XMLElement* node, const char* attr, float default_value = -1.0f) {
    float maybe_value = default_value;

    node->QueryFloatAttribute(attr, &maybe_value);

    return maybe_value;
}

static bool get_str_bool_attr(const tinyxml2::XMLElement* node, const char* attr, bool default_value = false) {
    const char* maybe_value = nullptr;

    node->QueryStringAttribute(attr, &maybe_value);

    if (maybe_value == nullptr) {
        return default_value;
    }

    if (strcmp("true", maybe_value) == 0) {
        return true;
    }

    if (strcmp("false", maybe_value) == 0) {
        return false;
    }
}

static int get_int_attr(const tinyxml2::XMLElement* node, const char* attr, int default_value = -1) {
    int maybe_value = default_value;

    node->QueryIntAttribute(attr, &maybe_value);

    return maybe_value;
}

static std::string get_string_attr(const tinyxml2::XMLElement* node, const char* attr) {
    const char* maybe_value;

    node->QueryStringAttribute(attr, &maybe_value);

    return maybe_value;
}

config::Translate::Translate(const tinyxml2::XMLElement* elem) :
    x(get_float_attr(elem, "x")),
    y(get_float_attr(elem, "y")),
    z(get_float_attr(elem, "z")) {}

config::Scale::Scale(const tinyxml2::XMLElement* elem) :
    x(get_float_attr(elem, "x")),
    y(get_float_attr(elem, "y")),
    z(get_float_attr(elem, "z")) {}

config::Rotate::Rotate(const tinyxml2::XMLElement* elem) :
    time(get_int_attr(elem, "time", 0.0f)),
    angle(get_float_attr(elem, "angle", 0.0f)),
    x(get_float_attr(elem, "x")),
    y(get_float_attr(elem, "y")),
    z(get_float_attr(elem, "z")) {}

config::CatmullRomTranslate::CatmullRomTranslate(const tinyxml2::XMLElement* elem) :
    time(get_float_attr(elem, "time", 0.0f)),
    align(get_str_bool_attr(elem, "align", false)),
    points([elem]() {
        std::vector<Point> pts;
        const tinyxml2::XMLElement* point_elem = elem->FirstChildElement("point");

        while (point_elem != nullptr) {
            pts.emplace_back(
                get_float_attr(point_elem, "x"),
                get_float_attr(point_elem, "y"),
                get_float_attr(point_elem, "z")
            );

            point_elem = point_elem->NextSiblingElement("point");
        }

        return pts;
    }()) {}

config::Transform::Transform(config::Translate* translate) :
    is_translate(true),
    translate(translate) {}

config::Transform::Transform(config::Rotate *rotate) :
    is_rotate(true),
    rotate(rotate) {}

config::Transform::Transform(config::Scale *scale) :
    is_scale(true),
    scale(scale) {}

config::Transform::Transform(config::CatmullRomTranslate *cr_translate) :
    is_cr_translate(true),
    cr_translate(cr_translate) {}

config::PointLight::PointLight(const tinyxml2::XMLElement* elem) :
    pos_x(get_float_attr(elem, "posX", get_float_attr(elem, "posx", 0.0f))),
    pos_y(get_float_attr(elem, "posY", get_float_attr(elem, "posy", 0.0f))),
    pos_z(get_float_attr(elem, "posZ", get_float_attr(elem, "posz", 0.0f))),
    r(get_float_attr(elem, "R", get_float_attr(elem, "r", 255)) / 255.0f),
    g(get_float_attr(elem, "G", get_float_attr(elem, "g", 255)) / 255.0f),
    b(get_float_attr(elem, "B", get_float_attr(elem, "b", 255)) / 255.0f) {}

config::DirectionalLight::DirectionalLight(const tinyxml2::XMLElement* elem) :
    dir_x(get_float_attr(elem, "dirX", get_float_attr(elem, "dirx", 0.0f))),
    dir_y(get_float_attr(elem, "dirY", get_float_attr(elem, "diry", -1.0f))),
    dir_z(get_float_attr(elem, "dirZ", get_float_attr(elem, "dirz", 0.0f))),
    r(get_float_attr(elem, "R", get_float_attr(elem, "r", 255)) / 255.0f),
    g(get_float_attr(elem, "G", get_float_attr(elem, "g", 255)) / 255.0f),
    b(get_float_attr(elem, "B", get_float_attr(elem, "b", 255)) / 255.0f) {}

config::SpotLight::SpotLight(const tinyxml2::XMLElement* elem) :
    pos_x(get_float_attr(elem, "posX", get_float_attr(elem, "posx", 0.0f))),
    pos_y(get_float_attr(elem, "posY", get_float_attr(elem, "posy", 0.0f))),
    pos_z(get_float_attr(elem, "posZ", get_float_attr(elem, "posz", 0.0f))),
    dir_x(get_float_attr(elem, "dirX", get_float_attr(elem, "dirx", 0.0f))),
    dir_y(get_float_attr(elem, "dirY", get_float_attr(elem, "diry", -1.0f))),
    dir_z(get_float_attr(elem, "dirZ", get_float_attr(elem, "dirz", 0.0f))),
    r(get_float_attr(elem, "R", get_float_attr(elem, "r", 255)) / 255.0f),
    g(get_float_attr(elem, "G", get_float_attr(elem, "g", 255)) / 255.0f),
    b(get_float_attr(elem, "B", get_float_attr(elem, "b", 255)) / 255.0f),
    cutoff(get_float_attr(elem, "cutoff", 45.0f)),
    exponent(get_float_attr(elem, "exponent", 1.0f)) {}

config::Light::Light(SpotLight* sl) : is_spotlight(true), spotlight(sl) {}
config::Light::Light(PointLight* pl) : is_point(true), point(pl) {}
config::Light::Light(DirectionalLight* dl) : is_directional(true), directional(dl) {}

config::Colour::Colour(const tinyxml2::XMLElement* elem) :
    r(get_int_attr(elem, "R", get_int_attr(elem, "r", 0))),
    g(get_int_attr(elem, "G", get_int_attr(elem, "g", 0))),
    b(get_int_attr(elem, "B", get_int_attr(elem, "b", 0))) {}

config::Colour::Colour(const int r, const int g, const int b) :
    r(r),
    g(g),
    b(b) {}

config::ModelColour::ModelColour(const tinyxml2::XMLElement* colour_elem) : ModelColour() {
    const tinyxml2::XMLElement* diffuse_elem = get_xml_element(colour_elem, "diffuse");
    const tinyxml2::XMLElement* ambient_elem = get_xml_element(colour_elem, "ambient");
    const tinyxml2::XMLElement* specular_elem = get_xml_element(colour_elem, "specular");
    const tinyxml2::XMLElement* emissive_elem = get_xml_element(colour_elem, "emissive");
    const tinyxml2::XMLElement* shininess_elem = get_xml_element(colour_elem, "shininess");

    diffuse = diffuse_elem ? Colour(diffuse_elem) : diffuse;
    ambient = ambient_elem ? Colour(ambient_elem) : ambient;
    specular = specular_elem ? Colour(specular_elem) : specular;
    emissive = emissive_elem ? Colour(emissive_elem) : emissive;
    shininess = shininess_elem ? get_float_attr(shininess_elem, "value", 0.0f) : shininess;
}

config::ModelColour::ModelColour() :
    diffuse(Colour(200, 200, 200)),
    ambient(Colour(50, 50, 50)),
    specular(Colour(0, 0, 0)),
    emissive(Colour(0, 0, 0)),
    shininess(0.0f) {}

config::Model::Model(const tinyxml2::XMLElement* elem) :
    file(get_string_attr(elem, "file"))
{
    const tinyxml2::XMLElement* texture_elem = elem->FirstChildElement("texture");
    const tinyxml2::XMLElement* colour_elem = elem->FirstChildElement("color");

    if (texture_elem) {
        texture_file = get_string_attr(texture_elem, "file");
    }

    if (colour_elem) {
        colour = ModelColour(colour_elem);
    }
}

config::Group::Group(const tinyxml2::XMLElement* elem) {
    auto maybe_models = elem->FirstChildElement("models");

    if (maybe_models != nullptr) {
        auto curr_elem = maybe_models->FirstChildElement("model");

        while (curr_elem) {
            if (strcmp("model", curr_elem->Name()) == 0) {
                models.emplace_back(curr_elem);
            }

            curr_elem = curr_elem->NextSiblingElement();
        }
    }

    auto maybe_transform = elem->FirstChildElement("transform");

    if (maybe_transform != nullptr) {
        auto curr_elem = maybe_transform->FirstChildElement();

        while (curr_elem) {
            if (strcmp("translate", curr_elem->Name()) == 0) {
                const char* time_attr = curr_elem->Attribute("time");

                if (time_attr != nullptr) {
                    transforms.push_back(Transform(new CatmullRomTranslate(curr_elem)));
                }
                else {
                    transforms.push_back(Transform(new Translate(curr_elem)));
                }
            }
            else if (strcmp("scale", curr_elem->Name()) == 0) {
                transforms.push_back(Transform(new Scale(curr_elem)));
            }
            else if (strcmp("rotate", curr_elem->Name()) == 0) {
                transforms.push_back(Transform(new Rotate(curr_elem)));
            }

            curr_elem = curr_elem->NextSiblingElement();
        }
    }

    auto maybe_group = elem->FirstChildElement("group");

    while (maybe_group != nullptr) {
        groups.emplace_back(maybe_group);

        maybe_group = maybe_group->NextSiblingElement("group");
    }
}

config::Coords::Coords(const tinyxml2::XMLElement* elem) :
    x(get_float_attr(elem, "x")),
    y(get_float_attr(elem, "y")),
    z(get_float_attr(elem, "z")) {}

config::Projection::Projection(const tinyxml2::XMLElement* elem) :
    near(get_int_attr(elem, "near")),
    fov(get_int_attr(elem, "fov")),
    far(get_int_attr(elem, "far")) {}

config::Camera::Camera(const tinyxml2::XMLElement* elem) :
    projection(Projection(get_xml_element(elem, "projection"))),
    position(Coords(get_xml_element(elem, "position"))),
    look_at(Coords(get_xml_element(elem, "lookAt"))),
    up(Coords(get_xml_element(elem, "up"))) {}

config::Window::Window(const tinyxml2::XMLElement* elem) :
    height(get_int_attr(elem, "height")),
    width(get_int_attr(elem, "width")) {}

config::Skybox::Skybox(const tinyxml2::XMLElement* elem) :
    right(get_string_attr(elem, "right")),
    left(get_string_attr(elem, "left")),
    top(get_string_attr(elem, "top")),
    bottom(get_string_attr(elem, "bottom")),
    front(get_string_attr(elem, "front")),
    back(get_string_attr(elem, "back")) {}

config::World::World(const tinyxml2::XMLElement* elem) :
    camera(Camera(get_xml_element(elem, "camera"))),
    window(Window(get_xml_element(elem, "window"))),
    group(Group(get_xml_element(elem, "group")))
{
    const tinyxml2::XMLElement* lights_node = elem->FirstChildElement("lights");

    if (lights_node) {
        const tinyxml2::XMLElement* light_elem = lights_node->FirstChildElement("light");

        while (light_elem) {
            const char* type_str = nullptr;
            light_elem->QueryStringAttribute("type", &type_str);

            if (type_str) {
                if (strcmp(type_str, "point") == 0) {
                    lights.emplace_back(new PointLight(light_elem));
                } else if (strcmp(type_str, "directional") == 0) {
                    lights.emplace_back(new DirectionalLight(light_elem));
                } else if (strcmp(type_str, "spot") == 0) {
                    lights.emplace_back(new SpotLight(light_elem));
                } else {
                    throw std::invalid_argument(std::string("Unknown light type ", type_str));
                }
            }

            light_elem = light_elem->NextSiblingElement("light");
        }
    }

    const tinyxml2::XMLElement* skybox_node = elem->FirstChildElement("skybox");

    if (skybox_node != nullptr) {
        skybox = Skybox(skybox_node);
    }
}

config::EngineConfig::EngineConfig(const std::string base_path, const tinyxml2::XMLDocument* doc) :
    base_path(base_path),
    world(World(get_xml_element(doc, "world"))) {}