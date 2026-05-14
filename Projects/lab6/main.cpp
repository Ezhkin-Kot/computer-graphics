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

std::vector<ssu::Model> readFromFile(const char *fileName, Screen &screen) {
    std::ifstream in(fileName);
    std::vector<ssu::Model> models;
    Mat4 M = Mat4(1.f);
    Mat4 initM;
    std::vector<ssu::Path> figure;
    std::vector<Mat4> transforms;
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
        std::string cmd; // Переменная для имени команды
        s >> cmd;        // Считываем имя команды
        if (cmd == "camera") {
            // Координаты точки наблюдения
            s >> screen.S.x >> screen.S.y >> screen.S.z;
            // Точка, в которую направлен вектор наблюдения
            s >> screen.P.x >> screen.P.y >> screen.P.z;
            // Вектор направления вверх
            s >> screen.u.x >> screen.u.y >> screen.u.z;
        } else if (cmd == "screen") {
            s >> screen.fovy_work >> screen.aspect >> screen.near >> screen.far;
            screen.fovy = screen.fovy_work / 180.f * PI;
        } else if (cmd == "color") {     // Цвет линии
            s >> r >> g >> b;            // Считываем три компоненты цвета
        } else if (cmd == "thickness") { // Толщина линии
            s >> thickness;              // Считываем значение толщины
        } else if (cmd == "path") {      // Набор точек
            std::vector<Vec3> vertices;  // Список точек ломаной
            int n;                       // Количество точек
            s >> n;
            std::string str1; // Дополнительная строка для чтения из файла
            while (n > 0) {   // Пока не все точки считаны
                getline(in, str1);
                if (isIgnorableLine(str1)) {
                    continue;
                }
                float x, y, z;
                std::stringstream s1(str1);
                s1 >> x >> y >> z;
                vertices.push_back(Vec3(x, y, z)); // Добавляем точку в список
                --n;
            }
            // Все точки считаны, генерируем ломаную (path) и кладем ее в список
            figure.push_back(ssu::Path(vertices,
                                       Color{static_cast<uint8_t>(r),
                                             static_cast<uint8_t>(g),
                                             static_cast<uint8_t>(b), 255},
                                       thickness));
        } else if (cmd == "model") {
            float mVcx, mVcy, mVcz, mVx, mVy, mVz; // Параметры команды model
            // Считываем значения переменных
            s >> mVcx >> mVcy >> mVcz >> mVx >> mVy >> mVz;
            float S = mVx / mVy < 1 ? 2.f / mVy : 2.f / mVx;
            // Сдвиг точки привязки из начала координат в нужную позицию
            // После которого проводим масштабирование
            initM = scale(S, S, S) * translate(-mVcx, -mVcy, -mVcz);
            figure.clear();
        } else if (cmd == "figure") {
            models.push_back(ssu::Model(figure, M * initM, Vx,
                                        Vy)); // Добавляем рисунок в список
        } else if (cmd == "translate") {
            float Tx, Ty, Tz;    // Параметры преобразования переноса
            s >> Tx >> Ty >> Tz; // Считываем параметры
            // Добавляем перенос к общему преобразованию
            M = translate(Tx, Ty, Tz) * M;
        } else if (cmd == "scale") {
            float S; // Параметр масштабирования
            s >> S;  // Считываем параметр
            // Добавляем масштабирование к общему преобразованию
            M = scale(S, S, S) * M;
        } else if (cmd == "rotate") {
            float theta;                  // Угол поворота в градусах
            float nx, ny, nz;             // Координаты вектора поворота
            s >> theta >> nx >> ny >> nz; // Считываем параметр
            // Добавляем поворот к общему преобразованию
            M = rotate(theta / 180.f * PI, Vec3(nx, ny, nz)) * M;
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

    std::vector<ssu::Model> models;
    Screen s;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        s.initWorkPars();
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

        // keymap_handler(s);

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
