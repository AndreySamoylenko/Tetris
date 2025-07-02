/*
 * ТЕТРИС - Визуальная версия (только отображение)
 *
 * Особенности:
 * - Окно 480x600 пикселей
 * - Игровое поле 280x560 в позиции (30,30)
 * - Область предпросмотра 126x126 в позиции (330,30)
 * - Трёхслойные блоки: белый 28x28 -> чёрный 26x26 -> белый 24x24
 * - Отображение счета и статистики
 * - Статичные примеры фигур (без логики игры)
 *
 * Управление:
 * - Пробел: смена демонстрационных фигур
 * - Escape: выход
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>   // для rand() и srand()
#include <ctime>     // для time()
#include <string>    // для работы со строками
#include <sstream>   // для преобразования чисел в строки

 // ==============================================
 // КОНСТАНТЫ ИГРЫ
 // ==============================================

const int WINDOW_WIDTH = 480;    // Ширина окна
const int WINDOW_HEIGHT = 600;   // Высота окна
const int BLOCK_SIZE = 28;       // Размер одного блока тетромино
const int FIELD_WIDTH = 10;      // Ширина игрового поля в блоках
const int FIELD_HEIGHT = 20;     // Высота игрового поля в блоках
const int FIELD_X = 30;          // X-координата левого верхнего угла игрового поля
const int FIELD_Y = 30;          // Y-координата левого верхнего угла игрового поля
const int NEXT_X = 330;          // X-координата области предпросмотра
const int NEXT_Y = 30;           // Y-координата области предпросмотра

// ==============================================
// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
// ==============================================

// Двумерный массив игрового поля (0 = пусто, 1 = занято)
int field[FIELD_HEIGHT][FIELD_WIDTH];

// Массив с формами всех 7 типов тетромино
int tetrominos[7][4][4] = {
    // I - прямая палка
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
    // O - квадрат  
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
    // T - тройка с выступом
    {{0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0}},
    // S - зигзаг влево
    {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
    // Z - зигзаг вправо
    {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
    // J - буква J
    {{0,0,0,0},{1,0,0,0},{1,1,1,0},{0,0,0,0}},
    // L - буква L
    {{0,0,0,0},{0,0,1,0},{1,1,1,0},{0,0,0,0}}
};

// ==============================================
// СТРУКТУРЫ ДАННЫХ
// ==============================================

// Структура для представления игровой фигуры
struct Piece {
    int type;     // Тип фигуры (0-6)
    int x, y;     // Позиция фигуры на игровом поле
};

// Структура для счета и статистики
struct GameStats {
    int score;           // Очки игрока
    int lines;           // Количество убранных линий
    int level;           // Уровень игры
    int pieces;          // Количество упавших фигур
};

// Текущая демонстрационная фигура
Piece currentPiece;
// Следующая фигура (показывается в области предпросмотра)
Piece nextPiece;
// Статистика игры
GameStats stats;
// Шрифт для отображения текста
sf::Font font;

// ==============================================
// ФУНКЦИИ
// ==============================================

/**
 * Инициализация игры
 */
void initGame() {
    // Очищаем всё игровое поле
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            field[i][j] = 0;
        }
    }

    // Создаем демонстрационные блоки на поле (нижние 3 ряда с пропусками)
    for (int i = FIELD_HEIGHT - 3; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (j != 2 && j != 7) { // Оставляем пропуски
                field[i][j] = 1;
            }
        }
    }

    srand(time(nullptr));

    // Создаём демонстрационные фигуры
    currentPiece.type = rand() % 7;
    currentPiece.x = FIELD_WIDTH / 2 - 2;
    currentPiece.y = 5;

    nextPiece.type = rand() % 7;
    nextPiece.x = 0;
    nextPiece.y = 0;

    // Инициализируем статистику
    stats.score = 12340;      // Демонстрационный счет
    stats.lines = 15;         // Демонстрационное количество линий
    stats.level = 3;          // Демонстрационный уровень
    stats.pieces = 47;        // Демонстрационное количество фигур
}

/**
 * Инициализация шрифта
 */
bool initFont() {
    // Пытаемся загрузить системный шрифт Windows
    return font.loadFromFile("C:/Windows/Fonts/arial.ttf");
}

/**
 * Рисование трёхслойного блока
 */
void drawBlock(sf::RenderWindow& window, int x, int y) {
    // Слой 1: Белый внешний квадрат 28x28
    sf::RectangleShape outerBlock(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    outerBlock.setPosition(x, y);
    outerBlock.setFillColor(sf::Color::White);
    window.draw(outerBlock);

    // Слой 2: Чёрный средний квадрат 26x26
    sf::RectangleShape middleBlock(sf::Vector2f(26, 26));
    middleBlock.setPosition(x + 1, y + 1);
    middleBlock.setFillColor(sf::Color::Black);
    window.draw(middleBlock);

    // Слой 3: Белый внутренний квадрат 24x24
    sf::RectangleShape innerBlock(sf::Vector2f(24, 24));
    innerBlock.setPosition(x + 2, y + 2);
    innerBlock.setFillColor(sf::Color::White);
    window.draw(innerBlock);
}

/**
 * Рисование текста
 */
void drawText(sf::RenderWindow& window, const std::string& text, float x, float y,
    unsigned int size = 18, sf::Color color = sf::Color::White) {
    sf::Text textObj;
    textObj.setFont(font);
    textObj.setString(text);
    textObj.setCharacterSize(size);
    textObj.setFillColor(color);
    textObj.setPosition(x, y);
    window.draw(textObj);
}

/**
 * Смена демонстрационных фигур
 */
void changeDemoFigures() {
    currentPiece.type = rand() % 7;
    nextPiece.type = rand() % 7;

    // Увеличиваем демонстрационную статистику
    stats.pieces++;
    stats.score += 100;
    if (stats.pieces % 10 == 0) {
        stats.lines++;
        stats.score += 500;
    }
    if (stats.lines % 10 == 0) {
        stats.level++;
    }
}

/**
 * Преобразование числа в строку
 */
std::string toString(int number) {
    std::stringstream ss;
    ss << number;
    return ss.str();
}

// ==============================================
// ОСНОВНАЯ ФУНКЦИЯ
// ==============================================

int main() {
    // Создаём окно
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris - Visual Demo");

    // Инициализируем игру
    initGame();

    // Пытаемся загрузить шрифт
    bool fontLoaded = initFont();

    // Основной цикл отображения
    while (window.isOpen()) {
        // Обработка событий
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Space:
                    // Пробел - смена демонстрационных фигур
                    changeDemoFigures();
                    break;
                case sf::Keyboard::Escape:
                    // Escape - выход
                    window.close();
                    break;
                default:
                    break;
                }
            }
        }

        // Отрисовка
        window.clear(sf::Color::Black);

        // Рисуем границу основного игрового поля
        sf::RectangleShape fieldBorder(sf::Vector2f(280, 560));
        fieldBorder.setPosition(FIELD_X, FIELD_Y);
        fieldBorder.setFillColor(sf::Color::Transparent);
        fieldBorder.setOutlineThickness(2);
        fieldBorder.setOutlineColor(sf::Color::White);
        window.draw(fieldBorder);

        // Рисуем границу области предпросмотра
        sf::RectangleShape nextBorder(sf::Vector2f(126, 126));
        nextBorder.setPosition(NEXT_X, NEXT_Y);
        nextBorder.setFillColor(sf::Color::Transparent);
        nextBorder.setOutlineThickness(2);
        nextBorder.setOutlineColor(sf::Color::White);
        window.draw(nextBorder);

        // Рисуем размещённые блоки на игровом поле
        for (int i = 0; i < FIELD_HEIGHT; i++) {
            for (int j = 0; j < FIELD_WIDTH; j++) {
                if (field[i][j]) {
                    int blockX = FIELD_X + j * BLOCK_SIZE;
                    int blockY = FIELD_Y + i * BLOCK_SIZE;
                    drawBlock(window, blockX, blockY);
                }
            }
        }

        // Рисуем текущую демонстрационную фигуру
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (tetrominos[currentPiece.type][i][j]) {
                    int blockX = FIELD_X + (currentPiece.x + j) * BLOCK_SIZE;
                    int blockY = FIELD_Y + (currentPiece.y + i) * BLOCK_SIZE;
                    drawBlock(window, blockX, blockY);
                }
            }
        }

        // Рисуем следующую фигуру в области предпросмотра
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (tetrominos[nextPiece.type][i][j]) {
                    int blockX = NEXT_X + 20 + j * BLOCK_SIZE;
                    int blockY = NEXT_Y + 20 + i * BLOCK_SIZE;
                    drawBlock(window, blockX, blockY);
                }
            }
        }

        // ОТРИСОВКА ИНТЕРФЕЙСА И СЧЕТА
        if (fontLoaded) {
            // Заголовок "Next"
            drawText(window, "NEXT", NEXT_X + 35, NEXT_Y - 25, 16, sf::Color::White);

            // Статистика справа от области Next
            int statsX = NEXT_X;
            int statsY = NEXT_Y + 140;

            // Счет
            drawText(window, "SCORE", statsX, statsY, 14, sf::Color::Yellow);
            drawText(window, toString(stats.score), statsX, statsY + 20, 16, sf::Color::White);

            // Линии
            drawText(window, "LINES", statsX, statsY + 50, 14, sf::Color::Yellow);
            drawText(window, toString(stats.lines), statsX, statsY + 70, 16, sf::Color::White);

            // Уровень
            drawText(window, "LEVEL", statsX, statsY + 100, 14, sf::Color::Yellow);
            drawText(window, toString(stats.level), statsX, statsY + 120, 16, sf::Color::White);

            // Количество фигур
            drawText(window, "PIECES", statsX, statsY + 150, 14, sf::Color::Yellow);
            drawText(window, toString(stats.pieces), statsX, statsY + 170, 16, sf::Color::White);

            // Инструкции внизу
            drawText(window, "SPACE - Change Figures", 50, WINDOW_HEIGHT - 60, 12, sf::Color::Cyan);
            drawText(window, "ESC - Exit", 50, WINDOW_HEIGHT - 40, 12, sf::Color::Cyan);
        }
        else {
            // Если шрифт не загрузился, показываем рамку для статистики
            sf::RectangleShape scoreBg(sf::Vector2f(100, 200));
            scoreBg.setPosition(NEXT_X, NEXT_Y + 140);
            scoreBg.setFillColor(sf::Color(50, 50, 50));
            scoreBg.setOutlineThickness(1);
            scoreBg.setOutlineColor(sf::Color::White);
            window.draw(scoreBg);
        }

        // Отображаем всё нарисованное на экране
        window.display();
    }

    return 0;
}