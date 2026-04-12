#pragma once

#include "matrix.hpp"
#include <algorithm>

inline unsigned int codeKS(Vec2 P, float minX, float minY, float maxX,
                           float maxY) {
    unsigned int code = 0;

    if (P.x < minX) {
        code += 1;
    } else if (P.x > maxX) {
        code += 2;
    }
    if (P.y < minY) {
        code += 4;
    } else if (P.y > maxY) {
        code += 8;
    }
    return code;
}

inline bool clip(Vec2 &A, Vec2 &B, float minX, float minY, float maxX,
                 float maxY) {
    unsigned int codeA = codeKS(A, minX, minY, maxX, maxY);
    unsigned int codeB = codeKS(B, minX, minY, maxX, maxY);
    while (codeA | codeB) {
        if (codeA & codeB) { // поразрядное И не равно нулю
            return false;    // отрезок полностью невидим
        }
        if (codeA == 0) {            // если A видима, то B невидима
            std::swap(A, B);         // меняем местами A и B
            std::swap(codeA, codeB); // меняем местами их коды
        }
        if (codeA & 1) { // точка A левее области видимости
            A.y = A.y + (B.y - A.y) * (minX - A.x) / (B.x - A.x);
            A.x = minX;
        } else if (codeA & 2) { // точка A правее области видимости
            A.y = A.y + (B.y - A.y) * (maxX - A.x) / (B.x - A.x);
            A.x = maxX;
        } else if (codeA & 4) { // точка A ниже области видимости
            A.x = A.x + (B.x - A.x) * (minY - A.y) / (B.y - A.y);
            A.y = minY;
        } else { // точка A выше области видимости
            A.x = A.x + (B.x - A.x) * (maxY - A.y) / (B.y - A.y);
            A.y = maxY;
        }
        codeA = codeKS(A, minX, minY, maxX, maxY);
    }
    return true;
}
