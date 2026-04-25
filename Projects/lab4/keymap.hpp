#pragma once

#include "matrix.hpp"
#include "screen.hpp"
#include "transform.hpp"
#include <raylib.h>

inline void keymap_handler(Screen &screen) {
    float cx = screen.Rx / 2.f + screen.left;
    float cy = screen.Ry / 2.f + screen.top;
    Mat3 &T = screen.T;

    if (IsKeyDown(KEY_Q)) {
        // Перенос начала координат в (Wcx, Wcy)
        T = translate(-cx, -cy) * T;
        // Поворот на -0.01 радиан относительно нового центра
        T = rotate(-0.01f) * T;
        T = translate(cx, cy) * T; // Перенос начала координат обратно
    }
    if (IsKeyDown(KEY_E)) {
        T = translate(-cx, -cy) * T;
        T = rotate(0.01f) * T; // Поворот на 0.01 радиан
        T = translate(cx, cy) * T;
    }
    if (IsKeyDown(KEY_R)) {
        T = translate(-cx, -cy) * T;
        T = rotate(0.05f) * T; // Поворот на 0.05 радиан
        T = translate(cx, cy) * T;
    }
    if (IsKeyDown(KEY_Y)) {
        T = translate(-cx, -cy) * T;
        T = rotate(-0.05f) * T; // Поворот на -0.05 радиан
        T = translate(cx, cy) * T;
    }

    if (IsKeyDown(KEY_W)) {
        T = translate(0, -1) * T;
    }
    if (IsKeyDown(KEY_S)) {
        T = translate(0, 1) * T;
    }
    if (IsKeyDown(KEY_D)) {
        T = translate(1, 0) * T;
    }
    if (IsKeyDown(KEY_A)) {
        T = translate(-1, 0) * T;
    }
    if (IsKeyDown(KEY_T)) {
        T = translate(0, -10) * T;
    }
    if (IsKeyDown(KEY_G)) {
        T = translate(0, 10) * T;
    }
    if (IsKeyDown(KEY_F)) {
        T = translate(-10, 0) * T;
    }
    if (IsKeyDown(KEY_H)) {
        T = translate(10, 0) * T;
    }

    if (IsKeyDown(KEY_Z)) {
        T = translate(-cx, -cy) * T;
        T = scale(1.1f) * T; // Увеличение в 1.1 раза
        T = translate(cx, cy) * T;
    }
    if (IsKeyDown(KEY_X)) {
        T = translate(-cx, -cy) * T;
        T = scale(1.0f / 1.1f) * T; // Уменьшение в 1.1 раза
        T = translate(cx, cy) * T;
    }

    if (IsKeyPressed(KEY_U)) {
        T = translate(-cx, -cy) * T;
        T = mirrorX() * T;
        T = translate(cx, cy) * T;
    }
    if (IsKeyPressed(KEY_J)) {
        T = translate(-cx, -cy) * T;
        T = mirrorY() * T;
        T = translate(cx, cy) * T;
    }

    if (IsKeyDown(KEY_I)) {
        T = translate(-cx, -cy) * T;
        T = scale(1.1f, 1.0f) * T;
        T = translate(cx, cy) * T;
    }
    if (IsKeyDown(KEY_K)) {
        T = translate(-cx, -cy) * T;
        T = scale(1.0f / 1.1f, 1.0f) * T;
        T = translate(cx, cy) * T;
    }
    if (IsKeyDown(KEY_O)) {
        T = translate(-cx, -cy) * T;
        T = scale(1.0f, 1.1f) * T;
        T = translate(cx, cy) * T;
    }
    if (IsKeyDown(KEY_L)) {
        T = translate(-cx, -cy) * T;
        T = scale(1.0f, 1.0f / 1.1f) * T;
        T = translate(cx, cy) * T;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        T = screen.initT;
    }
}
