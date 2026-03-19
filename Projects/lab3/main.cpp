#include "figure.hpp"
#include "matrix.hpp"
#include "raygui.h"
#include "transform.hpp"
#include <fstream>
#include <iostream>
#include <nfd.h>
#include <raylib.h>
#include <sstream>

const char *const LETTERS =
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
    "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

bool isIgnorableLine(const std::string &line) {
    return line.find_first_not_of(" \t\r\n") == std::string::npos ||
           line.front() == '#';
}

ssu::Figure readFromFile(const char *fileName) {
    std::ifstream in(fileName);
    ssu::Figure figure;
    int r, g, b;
    float thickness;
    std::string line; // Временная переменная, в которую считываются строки
    while (in) {
        // Считываем очередную строку
        getline(in, line);
        if (isIgnorableLine(line)) {
            continue;
        }
        std::stringstream s(line);
        std::string cmd;      // Переменная для имени команды
        s >> cmd;             // Считываем имя команды
        if (cmd == "frame") { // Размеры изображения
            s >> figure.Vx >> figure.Vy;
            std::cout << figure.Vx << ' ' << figure.Vy << std::endl;
        } else if (cmd == "color") {     // Цвет линии
            s >> r >> g >> b;            // Считываем три компоненты цвета
        } else if (cmd == "thickness") { // Толщина линии
            s >> thickness;              // Считываем значение толщины
        } else if (cmd == "path") {      // Набор точек
            std::vector<Vec2> vertices;  // Список точек ломаной
            int n;                       // Количество точек
            s >> n;
            std::string str1; // Дополнительная строка для чтения из файла
            while (n > 0) {   // Пока не все точки считаны
                getline(in, str1);
                if (isIgnorableLine(str1)) {
                    continue;
                }
                float x, y;
                std::stringstream s1(str1);
                s1 >> x >> y;
                vertices.push_back(Vec2(x, y)); // Добавляем точку в список
                --n;
            }
            // Все точки считаны, генерируем ломаную (path) и кладем ее в список
            figure.paths.push_back(ssu::Path(
                vertices,
                Color{static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                      static_cast<uint8_t>(b), 255},
                thickness));
        }
    }
    return figure;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(600, 480, "Lab 3");
    SetTargetFPS(60);

    int cnt = 0;
    int *codepoints = LoadCodepoints(LETTERS, &cnt);
    Font f = LoadFontEx("Assets/Fonts/JetBrainsMono-Regular.ttf", 100,
                        codepoints, cnt);
    ssu::Figure figure;
    Mat3 T = Mat3(1.f); // Матрица, в которой накапливаются все преобразования
                        // первоначально - единичная матрица
    Mat3 initT;         // Матрица начального преобразования

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        const float Wx = static_cast<float>(GetScreenWidth());
        const float Wy = static_cast<float>(GetScreenHeight());
        const float Wcx = Wx / 2.0f;
        const float Wcy = Wy / 2.0f;
        const float windowAspect = Wx / Wy;

        if (GuiButton({Wx - 140, 20, 120, 30}, "OPEN FILE")) {
            nfdchar_t *outPath;
            nfdfilteritem_t filterItem[2] = {{"Text files", "txt"},
                                             {"All files", "*"}};
            nfdresult_t result =
                NFD_OpenDialog(&outPath, filterItem, 2, nullptr);

            if (result == NFD_OKAY) {
                figure = readFromFile(outPath);
                const float figureAspect = figure.Vx / figure.Vy;
                const float S = figureAspect < windowAspect ? Wy / figure.Vy
                                                            : Wx / figure.Vx;
                const float Ty =
                    S * figure.Vy; // Смещение в положительную сторону по
                                   // оси Oy после смены знака

                // Преобразования применяются справа-налево. Сначала
                // масштабирование, а потом перенос.
                // В initT совмещаем эти два преобразования.
                initT = translate(0, Ty) * scale(S, -S);
                T = initT;
                NFD_FreePath(outPath);
            } else if (result == NFD_CANCEL) {
                std::cerr << "INFO: NFD: user pressed cancel" << std::endl;
            } else {
                std::cerr << "ERROR: " << NFD_GetError() << std::endl;
            }
        }

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

        if (IsKeyPressed(KEY_C)) {
            T = initT;
        }

        Mat3 M = T * initT; // Совмещение начального преобразования и
                            // накопленных преобразований
        for (const auto &lines : figure.paths) {
            Vec2 start = normalize(T * Vec3(lines.vertices[0], 1));
            for (const auto &line : lines.vertices) {
                const Vec2 end = normalize(T * Vec3(line, 1));
                DrawLineEx({start.x, start.y}, {end.x, end.y}, lines.thickness,
                           lines.color);
                start = end;
            }
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
