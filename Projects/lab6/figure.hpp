#pragma once

#include "matrix.hpp"
#include <raylib.h>
#include <vector>

namespace ssu {
struct Path {
    std::vector<Vec2> vertices;
    Color color;
    float thickness;

    Path(std::vector<Vec2> vertices, Color color, float thickness)
        : vertices(vertices),
          color(color),
          thickness(thickness) {}
};

struct Figure {
    std::vector<Path> paths;
    Mat3 modelM;
    float Vx;
    float Vy;

    Figure() = default;
    Figure(std::vector<Path> paths, Mat3 mat, float Vx, float Vy)
        : paths(paths),
          modelM(mat),
          Vx(Vx),
          Vy(Vy) {}
};
} // namespace ssu
