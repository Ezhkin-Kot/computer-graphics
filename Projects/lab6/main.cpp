#include "clip.hpp"
#include "figure.hpp"
#include "keymap.hpp"
#include "matrix.hpp"
#include "raygui.h"
#include "screen.hpp"
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

std::vector<ssu::Figure> readFromFile(const char *fileName, Screen &screen) {
    std::ifstream in(fileName);
    std::vector<ssu::Figure> models;
    Mat3 M = Mat3(1.f);
    Mat3 initM;
    std::vector<ssu::Path> figure;
    std::vector<Mat3> transforms;
    float Vx, Vy;
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
            s >> Vx >> Vy;
            float figAspect = Vx / Vy;
            Mat3 T1 = translate(-Vx / 2, -Vy / 2);
            float S =
                figAspect < screen.rectAspect ? screen.Ry / Vy : screen.Rx / Vx;
            Mat3 S1 = scale(S, -S);
            Mat3 T2 = translate(screen.Rcx + screen.Rx / 2,
                                screen.Rcy - screen.Ry / 2);
            screen.initT = T2 * (S1 * T1);
            screen.T = screen.initT;
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
            figure.push_back(ssu::Path(vertices,
                                       Color{static_cast<uint8_t>(r),
                                             static_cast<uint8_t>(g),
                                             static_cast<uint8_t>(b), 255},
                                       thickness));
        } else if (cmd == "model") {
            float mVcx, mVcy, mVx, mVy;      // Параметры команды model
            s >> mVcx >> mVcy >> mVx >> mVy; // Считываем значения переменных
            float S = mVx / mVy < 1 ? 2.f / mVy : 2.f / mVx;
            // Сдвиг точки привязки из начала координат в нужную позицию
            // После которого проводим масштабирование
            initM = scale(S) * translate(-mVcx, -mVcy);
            figure.clear();
        } else if (cmd == "figure") {
            models.push_back(ssu::Figure(figure, M * initM, Vx,
                                         Vy)); // Добавляем рисунок в список
        } else if (cmd == "translate") {
            float Tx, Ty;  // Параметры преобразования переноса
            s >> Tx >> Ty; // Считываем параметры
            // Добавляем перенос к общему преобразованию
            M = translate(Tx, Ty) * M;
        } else if (cmd == "scale") {
            float S; // Параметр масштабирования
            s >> S;  // Считываем параметр
            // Добавляем масштабирование к общему преобразованию
            M = scale(S) * M;
        } else if (cmd == "rotate") {
            float theta; // Угол поворота в градусах
            s >> theta;  // Считываем параметр
            // Добавляем поворот к общему преобразованию
            M = rotate(theta / 180.f * PI) * M;
        } else if (cmd == "pushTransform") {
            transforms.push_back(M); // сохраняем матрицу в стек
        } else if (cmd == "popTransform") {
            M = transforms.back(); // получаем верхний элемент стека
            transforms.pop_back(); // выкидываем матрицу из стека
        }
    }
    return models;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(600, 480, "Lab 4");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    SetWindowMinSize(155, 120);

    int cnt = 0;
    int *codepoints = LoadCodepoints(LETTERS, &cnt);
    Font f = LoadFontEx("Assets/Fonts/JetBrainsMono-Regular.ttf", 100,
                        codepoints, cnt);

    std::vector<ssu::Figure> models;
    Screen s;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        s.update();

        DrawRectangleLinesEx({s.minX, s.minY, s.Rx, s.Ry}, 2, BLACK);

        if (GuiButton({s.Wx - 140, 20, 120, 30}, "OPEN FILE")) {
            nfdchar_t *outPath;
            nfdfilteritem_t filterItem[2] = {{"Text files", "txt"},
                                             {"All files", "*"}};
            nfdresult_t result =
                NFD_OpenDialog(&outPath, filterItem, 2, nullptr);

            if (result == NFD_OKAY) {
                models = readFromFile(outPath, s);
                NFD_FreePath(outPath);
            } else if (result == NFD_CANCEL) {
                std::cerr << "INFO: NFD: user pressed cancel" << std::endl;
            } else {
                std::cerr << "ERROR: " << NFD_GetError() << std::endl;
            }
        }

        keymap_handler(s);

        for (const auto &figure : models) {
            Mat3 TM = s.T * figure.modelM;
            for (const auto &lines : figure.paths) {
                Vec2 start = normalize(TM * Vec3(lines.vertices[0], 1));
                for (const auto &line : lines.vertices) {
                    Vec2 end = normalize(TM * Vec3(line, 1));
                    Vec2 tmpEnd = end;
                    if (clip(start, end, s.minX, s.minY, s.maxX, s.maxY)) {
                        DrawLineEx({start.x, start.y}, {end.x, end.y},
                                   lines.thickness, lines.color);
                    }
                    start = tmpEnd;
                }
            }
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
