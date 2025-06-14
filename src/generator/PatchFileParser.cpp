#include "PatchFileParser.h"

#include <fstream>
#include <sstream>
#include <model/Point.h>

bool PatchFileParser::read_line(std::istream& stream, std::string& line, int& line_num) {
    if (!std::getline(stream, line)) {
        if (stream.eof()) {
            return false;
        }

        throw std::runtime_error(
            "Error reading line " + std::to_string(line_num) + " from patch file."
        );
    }

    line_num++;
    return true;
}

template <typename T>
std::vector<T> PatchFileParser::parse_comma_separated(const std::string& line, size_t expected_count) {
    std::vector<T> values;
    values.reserve(expected_count);
    std::stringstream ss(line);
    std::string segment;
    T value;

    while (std::getline(ss, segment, ',')) {
        std::stringstream segment_ss(segment);
        segment_ss >> value;

        if (segment_ss.fail() || !segment_ss.eof()) {
             throw std::runtime_error("Failed to parse value '" + segment + "' from line: " + line);
        }

        values.push_back(value);
    }

    return values;
}


std::vector<BezierPatch> PatchFileParser::parse(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open patch file: " + filename);
    }

    std::vector<BezierPatch> patches;
    std::vector<Point> all_control_points;
    std::string line;
    int line_num = 0;

    if (!read_line(file, line, line_num)) {
        throw std::runtime_error("Error");
    }

    int num_patches = std::stoi(line);
    patches.reserve(num_patches);

    std::vector<std::vector<unsigned int>> patch_indices_list(num_patches);
    for (int i = 0; i < num_patches; ++i) {
        if (!read_line(file, line, line_num)) {
            throw std::runtime_error("Error ");
        }

        std::vector<unsigned int> indices_1_based = parse_comma_separated<unsigned int>(line, 16);

        patch_indices_list[i].resize(16);

        for (size_t j = 0; j < 16; ++j) {
            patch_indices_list[i][j] = indices_1_based[j];
        }
    }

    if (!read_line(file, line, line_num)) {
        throw std::runtime_error("Error");
    }

    int num_control_points = std::stoi(line);
    all_control_points.reserve(num_control_points);

    for (int i = 0; i < num_control_points; ++i) {
        if (!read_line(file, line, line_num)) {
            throw std::runtime_error("Control point error " + std::to_string(i));
        }

        std::vector<float> coords = parse_comma_separated<float>(line, 3);
        all_control_points.emplace_back(Point{coords[0], coords[1], coords[2]});
    }

    for (int i = 0; i < num_patches; ++i) {
        std::vector<Point> current_patch_points;
        current_patch_points.reserve(16);

        for (unsigned int index : patch_indices_list[i]) {
            current_patch_points.push_back(all_control_points[index]);
        }

        patches.emplace_back(BezierPatch(current_patch_points));
    }

    return patches;
}
