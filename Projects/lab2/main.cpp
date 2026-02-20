#include "figure.hpp"
#include "hare.hpp"
#include "mushroom.hpp"
#include <raylib.h>

const char *const LETTERS =
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
    "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(600, 480, "Lab 1");
    SetTargetFPS(60);

    int cnt = 0;
    int *codepoints = LoadCodepoints(LETTERS, &cnt);
    Font f = LoadFontEx("Assets/Fonts/JetBrainsMono-Regular.ttf", 100,
                        codepoints, cnt);
    const ssu::Figure &defaultFigure = ssu::figure::HARE;
    const ssu::Figure &figure = ssu::figure::MUSHROOM;
    ssu::Figure currentFigure = figure;
    float figureAspect = currentFigure.Vx / currentFigure.Vy;
    bool keepAspect = true;
    bool setDefaultFigure = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        const float Wx = static_cast<float>(GetScreenWidth());
        const float Wy = static_cast<float>(GetScreenHeight());
        const float windowAspect = Wx / Wy;
        float S = figureAspect < windowAspect ? Wy / currentFigure.Vy
                                              : Wx / currentFigure.Vx;

        if (IsKeyPressed(KEY_M)) {
            keepAspect = !keepAspect;
        }

        if (IsKeyPressed(KEY_N)) {
            setDefaultFigure = !setDefaultFigure;
            if (setDefaultFigure) {
                currentFigure = defaultFigure;
            } else {
                currentFigure = figure;
            }
            figureAspect = currentFigure.Vx / currentFigure.Vy;
        }

        float Sx, Sy;
        if (keepAspect) {
            figureAspect = currentFigure.Vx / currentFigure.Vy;
            Sx = Sy = figureAspect < windowAspect ? Wy / currentFigure.Vy
                                                  : Wx / currentFigure.Vx;
        } else {
            Sx = Wx / currentFigure.Vx;
            Sy = Wy / currentFigure.Vy;
        }
        const float Ty =
            Sy * currentFigure.Vy; // смещение в положительную сторону по
                                   // оси Oy после смены знака
        for (size_t i = 0; i < currentFigure.vertices.size(); i += 4) {
            DrawLineEx({Sx * currentFigure.vertices[i],
                        Ty - Sy * currentFigure.vertices[i + 1]},
                       {Sx * currentFigure.vertices[i + 2],
                        Ty - Sy * currentFigure.vertices[i + 3]},
                       2, BLACK);
        };
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
