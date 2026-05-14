#pragma once

#include "matrix.hpp"
#include "screen.hpp"
#include <raylib.h>

inline void keymap_handler(Screen &s) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        s.initWorkPars();
    }
    if (IsKeyPressed(KEY_ONE)) {
        s.pType = s.ORTHO;
    }
    if (IsKeyPressed(KEY_TWO)) {
        s.pType = s.FRUSTUM;
    }
    if (IsKeyPressed(KEY_THREE)) {
        s.pType = s.PERSPECTIVE;
    }
    if (IsKeyDown(KEY_W)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            s.T =
                lookAt(Vec3(0, 0, -0.1f), Vec3(0, 0, -0.2f), Vec3(0, 1.f, 0)) *
                s.T;
        } else {
            s.T = lookAt(Vec3(0, 0, -1.f), Vec3(0, 0, -2.f), Vec3(0, 1.f, 0)) *
                  s.T;
        }
    }
    if (IsKeyDown(KEY_S)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            s.T =
                lookAt(Vec3(0, 0, 0.1f), Vec3(0, 0, 0), Vec3(0, 1.f, 0)) * s.T;
        } else {
            s.T = lookAt(Vec3(0, 0, 1.f), Vec3(0, 0, 0), Vec3(0, 1.f, 0)) * s.T;
        }
    }
    if (IsKeyDown(KEY_A)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            s.T = lookAt(Vec3(-0.1f, 0, 0), Vec3(-0.1f, 0, -1.f),
                         Vec3(0, 1.f, 0)) *
                  s.T;
        } else {
            s.T =
                lookAt(Vec3(-1.f, 0, 0), Vec3(-1.f, 0, -1.f), Vec3(0, 1.f, 0)) *
                s.T;
        }
    }
    if (IsKeyDown(KEY_D)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            s.T =
                lookAt(Vec3(0.1f, 0, 0), Vec3(0.1f, 0, -1.f), Vec3(0, 1.f, 0)) *
                s.T;
        } else {
            s.T = lookAt(Vec3(1.f, 0, 0), Vec3(1.f, 0, -1.f), Vec3(0, 1.f, 0)) *
                  s.T;
        }
    }
    if (IsKeyDown(KEY_R)) {
        Vec3 u_new = Mat3(rotate(0.1f, Vec3(0, 0, 1.f))) * Vec3(0, 1.f, 0);
        s.T = lookAt(Vec3(0, 0, 0), Vec3(0, 0, -1.f), u_new) * s.T;
    }
    if (IsKeyDown(KEY_T)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            Mat4 M = rotateP(0.1f, Vec3(1.f, 0, 0), Vec3(0, 0, -s.dist));
            Vec3 u_new = Mat3(M) * Vec3(0, 1.f, 0);
            Vec3 S_new = normalize(M * Vec4(0, 0, 0, 1.f));
            s.T = lookAt(S_new, Vec3(0, 0, -s.dist), u_new) * s.T;

        } else {
            Mat4 M = rotate(0.1f, Vec3(1.f, 0, 0));
            Vec3 u_new = Mat3(M) * Vec3(0, 1.f, 0);
            Vec3 P_new = normalize(M * Vec4(0, 0, -1.f, 1.f));
            s.T = lookAt({0, 0, 0}, P_new, u_new) * s.T;
        }
    }
    if (IsKeyDown(KEY_I)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            s.t -= 1;
        } else {
            s.t += 1;
        }
    }
    if (IsKeyDown(KEY_J)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            s.l -= 1;
        } else {
            s.l += 1;
        }
    }
}
