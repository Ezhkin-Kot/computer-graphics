#pragma once

#include "matrix.hpp"
#include <raylib.h>

struct Screen {
    float left = 30.0f, right = 100.0f, top = 20.0f, bottom = 50.0f;
    float minX, maxX;
    float minY, maxY;
    float Wx, Wy, Wcx, Wcy;
    float Rx, Ry, Rcx, Rcy;
    float rectAspect;

    Mat3 T = Mat3(1.f);
    Mat3 initT = Mat3(1.f);

    Screen() {
        Wx = static_cast<float>(GetScreenWidth());
        Wy = static_cast<float>(GetScreenHeight());
        Wcx = Wx / 2.0f;
        Wcy = Wy / 2.0f;
        minX = left;
        maxX = Wx - right;
        minY = top;
        maxY = Wy - bottom;
        Rx = maxX - minX;
        Ry = maxY - minY;
        Rcx = minX;
        Rcy = maxY;
        rectAspect = Rx / Ry;
    }
};
