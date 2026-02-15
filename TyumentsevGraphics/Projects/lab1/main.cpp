#include <raylib.h>

const char *const LETTERS =
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
    "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(600, 480, "Lab Uno");
    SetTargetFPS(60);

    int cnt = 0;
    int *codepoints = LoadCodepoints(LETTERS, &cnt);
    Font f = LoadFontEx("Assets/Fonts/JetBrainsMono-Regular.ttf", 100,
                        codepoints, cnt);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        const float Wx = static_cast<float>(GetScreenWidth());
        const float Wy = static_cast<float>(GetScreenHeight());

        DrawLineEx({0, 0}, {Wx, Wy}, 6, RED);
        DrawLineEx({90, 50}, {Wx, 80}, 10, BLUE);

        DrawLineEx({0, Wy / 3}, {Wx * 2 / 3, 0}, 5, GREEN);
        DrawLineEx({Wx * 2 / 3, 0}, {Wx, Wy * 2 / 3}, 5, GREEN);
        DrawLineEx({Wx, Wy * 2 / 3}, {Wx / 3, Wy}, 5, GREEN);
        DrawLineEx({Wx / 3, Wy}, {0, Wy / 3}, 5, GREEN);

        DrawTextEx(f, "Добро пожаловать на компьютерную графику", {40, 380}, 26,
                   0, BLACK);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
