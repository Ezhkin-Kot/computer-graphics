#pragma once

#include "matrix.hpp"
#include "transform.hpp"
#include <raylib.h>

inline void keymap_handler(Mat3 &T, Mat3 &initT, const float Wcx,
                           const float Wcy) {
    if (IsKeyDown(KEY_Q)) {
        // Перенос начала координат в (Wcx, Wcy)
        T = translate(-Wcx, -Wcy) * T;
        // Поворот на -0.01 радиан относительно нового центра
        T = rotate(-0.01f) * T;
        T = translate(Wcx, Wcy) * T; // Перенос начала координат обратно
    }
    if (IsKeyDown(KEY_E)) {
        T = translate(-Wcx, -Wcy) * T;
        T = rotate(0.01f) * T; // Поворот на 0.01 радиан
        T = translate(Wcx, Wcy) * T;
    }
    if (IsKeyDown(KEY_R)) {
        T = translate(-Wcx, -Wcy) * T;
        T = rotate(0.05f) * T; // Поворот на 0.05 радиан
        T = translate(Wcx, Wcy) * T;
    }
    if (IsKeyDown(KEY_Y)) {
        T = translate(-Wcx, -Wcy) * T;
        T = rotate(-0.05f) * T; // Поворот на -0.05 радиан
        T = translate(Wcx, Wcy) * T;
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
        T = translate(-Wcx, -Wcy) * T;
        T = scale(1.1f) * T; // Увеличение в 1.1 раза
        T = translate(Wcx, Wcy) * T;
    }
    if (IsKeyDown(KEY_X)) {
        T = translate(-Wcx, -Wcy) * T;
        T = scale(1.0f / 1.1f) * T; // Уменьшение в 1.1 раза
        T = translate(Wcx, Wcy) * T;
    }

    if (IsKeyPressed(KEY_U)) {
        T = translate(-Wcx, -Wcy) * T;
        T = mirrorX() * T;
        T = translate(Wcx, Wcy) * T;
    }
    if (IsKeyPressed(KEY_J)) {
        T = translate(-Wcx, -Wcy) * T;
        T = mirrorY() * T;
        T = translate(Wcx, Wcy) * T;
    }

    if (IsKeyDown(KEY_I)) {
        T = translate(-Wcx, -Wcy) * T;
        T = scale(1.1f, 1.0f) * T;
        T = translate(Wcx, Wcy) * T;
    }
    if (IsKeyDown(KEY_K)) {
        T = translate(-Wcx, -Wcy) * T;
        T = scale(1.0f / 1.1f, 1.0f) * T;
        T = translate(Wcx, Wcy) * T;
    }
    if (IsKeyDown(KEY_O)) {
        T = translate(-Wcx, -Wcy) * T;
        T = scale(1.0f, 1.1f) * T;
        T = translate(Wcx, Wcy) * T;
    }
    if (IsKeyDown(KEY_L)) {
        T = translate(-Wcx, -Wcy) * T;
        T = scale(1.0f, 1.0f / 1.1f) * T;
        T = translate(Wcx, Wcy) * T;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        T = initT;
    }
}
