#ifndef PATCHFILEPARSER_H
#define PATCHFILEPARSER_H

#include <string>
#include <vector>

#include "BezierPatch.h"

class PatchFileParser {
public:
    std::vector<BezierPatch> parse(const std::string& filename);

private:
    bool read_line(std::istream& stream, std::string& line, int& line_num);

    template <typename T>
    std::vector<T> parse_comma_separated(const std::string& line, size_t expected_count);
};


#endif //PATCHFILEPARSER_H
