#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "model/serialise/ObjSerialiser.h"
#include "model/Model.h"
#include "ImprovedPrimitiveGenerator.h"
#include "PatchFileParser.h"

void print_usage(const char* prog_name) {
    std::cerr << "Usage: " << prog_name << " <primitive> [options] <output_file.obj>" << std::endl;
    std::cerr << "Primitives and options:" << std::endl;
    std::cerr << "  plane <length> <divisions> <output_file>" << std::endl;
    std::cerr << "  box <length> <divisions> <output_file>" << std::endl;
    std::cerr << "  sphere <radius> <slices> <stacks> <output_file>" << std::endl;
    std::cerr << "  cone <radius> <height> <slices> <stacks> <output_file>" << std::endl;
    std::cerr << "  patch <input_patch_file> <tessellation_level>" << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string primitive_type = argv[1];
    ImprovedPrimitiveGenerator generator;

    try {
        if (primitive_type == "plane") {
            if (argc != 5) throw std::invalid_argument("Incorrect arguments for plane.");

            float length = std::stof(argv[2]);
            int divisions = std::stoi(argv[3]);
            std::string output_filename = argv[4];

            auto model = generator.plane(length, divisions);
            std::ofstream out_stream(output_filename);
            ObjSerialiser().serialise(model, out_stream);
        }
        else if (primitive_type == "box") {
            if (argc != 5) throw std::invalid_argument("Incorrect arguments for box.");

            float length = std::stof(argv[2]);
            int divisions = std::stoi(argv[3]);
            std::string output_filename = argv[4];

            auto model = generator.box(length, divisions);
            std::ofstream out_stream(output_filename);
            ObjSerialiser().serialise(model, out_stream);
        }
        else if (primitive_type == "sphere") {
            if (argc != 6) throw std::invalid_argument("Incorrect arguments for sphere.");

            float radius = std::stof(argv[2]);
            int slices = std::stoi(argv[3]);
            int stacks = std::stoi(argv[4]);
            std::string output_filename = argv[5];

            auto model = generator.sphere(radius, slices, stacks);
            std::ofstream out_stream(output_filename);
            ObjSerialiser().serialise(model, out_stream);
        }
        else if (primitive_type == "cone") {
            if (argc != 7) throw std::invalid_argument("Incorrect arguments for cone.");

            float radius = std::stof(argv[2]);
            float height = std::stof(argv[3]);
            int slices = std::stoi(argv[4]);
            int stacks = std::stoi(argv[5]);
            std::string output_filename = argv[6];

            auto model = generator.cone(radius, height, slices, stacks);
            std::ofstream out_stream(output_filename);
            ObjSerialiser().serialise(model, out_stream);
        }
        else if (primitive_type == "patch") {
            if (argc != 5) throw std::invalid_argument("Incorrect arguments for patch.");

            std::string input_patch_file = argv[2];
            int tessellation_level = std::stoi(argv[3]);
            std::string output_filename = argv[4];

            std::vector<BezierPatch> patches = PatchFileParser().parse(input_patch_file);

            auto model = generator.bezier_patches(patches, tessellation_level);
            std::ofstream out_stream(output_filename);
            ObjSerialiser().serialise(model, out_stream);

        }
        else {
            throw std::invalid_argument("Unknown type: " + primitive_type);
        }
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }

    return 0;
}