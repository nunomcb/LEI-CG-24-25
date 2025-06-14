#ifndef CG_CONFIG_H
#define CG_CONFIG_H

#include <tinyxml/tinyxml2.h>

#include <string>
#include <vector>
#include <optional>

#include "model/Vector.h"
#include "model/Point.h"

namespace config {
    struct Coords {
        explicit Coords(const tinyxml2::XMLElement*);

        const float x, y, z;

        Vector asVector() const;
    };

    inline Vector Coords::asVector() const {
        return {x, y, z};
    }

    typedef Coords Position, LookAt, Up;

    struct Projection {
        explicit Projection(const tinyxml2::XMLElement*);

        const int fov, far, near;
    };

    struct Camera {
        explicit Camera(const tinyxml2::XMLElement*);

        const Projection projection;
        const Position position;
        const LookAt look_at;
        const Up up;
    };

    struct Window {
        explicit Window(const tinyxml2::XMLElement*);

        const int width, height;
    };

    struct Colour {
        int r, g, b;
    
        explicit Colour(const tinyxml2::XMLElement* elem);
        explicit Colour(const int r, const int g, const int b);
    };

    struct ModelColour {
        Colour diffuse;
        Colour ambient;
        Colour specular;
        Colour emissive;
        float shininess;

        explicit ModelColour(const tinyxml2::XMLElement* colour_elem);
        explicit ModelColour();
    };

    struct Model {
        explicit Model(const tinyxml2::XMLElement*);

        std::string file;
        ModelColour colour;
        std::optional<std::string> texture_file;
    };

    struct Translate {
        explicit Translate(const tinyxml2::XMLElement*);

        const float x, y, z;
    };

    struct Scale {
        explicit Scale(const tinyxml2::XMLElement*);

        const float x, y, z;
    };

    struct Rotate {
        explicit Rotate(const tinyxml2::XMLElement*);

        const int time;
        const float angle, x, y, z;
    };

    struct CatmullRomTranslate {
        explicit CatmullRomTranslate(const tinyxml2::XMLElement*);
        const float time;
        const bool align = false;
        const std::vector<Point> points;
    };

    struct Transform {
        explicit Transform(CatmullRomTranslate* cr_translate);
        explicit Transform(Translate* translate);
        explicit Transform(Rotate* rotate);
        explicit Transform(Scale* scale);

        const bool is_cr_translate = false;
        const bool is_translate = false;
        const bool is_rotate = false;
        const bool is_scale = false;

        const CatmullRomTranslate* cr_translate = nullptr;
        const Translate* translate = nullptr;
        const Rotate* rotate = nullptr;
        const Scale* scale = nullptr;
    };

    struct Group {
        explicit Group(const tinyxml2::XMLElement*);

        std::vector<Transform> transforms;
        std::vector<Model> models;
        std::vector<Group> groups;
    };

    struct PointLight {
        float pos_x, pos_y, pos_z;
        float r, g, b;
    
        explicit PointLight(const tinyxml2::XMLElement* elem);
    };

    struct DirectionalLight {
        float dir_x, dir_y, dir_z;
        float r, g, b;
    
        explicit DirectionalLight(const tinyxml2::XMLElement* elem);
    };

    struct SpotLight {
        float pos_x, pos_y, pos_z;
        float dir_x, dir_y, dir_z;
        float r, g, b;
        float cutoff;
        float exponent;
    
        explicit SpotLight(const tinyxml2::XMLElement* elem);
    };

    struct Light {
        Light(SpotLight* sl);
        Light(PointLight* pl);
        Light(DirectionalLight* dl);

        bool is_point = false;
        bool is_spotlight = false;
        bool is_directional = false;
        
        SpotLight* spotlight = nullptr;
        PointLight* point = nullptr;
        DirectionalLight* directional = nullptr;
    };

    struct Skybox {
        std::string right;
        std::string left;
        std::string top;
        std::string bottom;
        std::string front;
        std::string back;
    
        explicit Skybox(const tinyxml2::XMLElement* elem);
    };

    struct World {
        explicit World(const tinyxml2::XMLElement*);

        const Window window;
        const Camera camera;
        const Group group;
        std::vector<Light> lights;
        std::optional<Skybox> skybox;
    };

    struct EngineConfig {
        explicit EngineConfig(std::string base_path, const tinyxml2::XMLDocument*);

        const std::string base_path;
        const World world;
    };
}

#endif //CG_CONFIG_H
