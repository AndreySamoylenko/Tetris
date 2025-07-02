/*
 * ������ - ���������� ������ (������ �����������)
 *
 * �����������:
 * - ���� 480x600 ��������
 * - ������� ���� 280x560 � ������� (30,30)
 * - ������� ������������� 126x126 � ������� (330,30)
 * - ���������� �����: ����� 28x28 -> ������ 26x26 -> ����� 24x24
 * - ����������� ����� � ����������
 * - ��������� ������� ����� (��� ������ ����)
 *
 * ����������:
 * - ������: ����� ���������������� �����
 * - Escape: �����
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>   // ��� rand() � srand()
#include <ctime>     // ��� time()
#include <string>    // ��� ������ �� ��������
#include <sstream>   // ��� �������������� ����� � ������

 // ==============================================
 // ��������� ����
 // ==============================================

const int WINDOW_WIDTH = 480;    // ������ ����
const int WINDOW_HEIGHT = 600;   // ������ ����
const int BLOCK_SIZE = 28;       // ������ ������ ����� ���������
const int FIELD_WIDTH = 10;      // ������ �������� ���� � ������
const int FIELD_HEIGHT = 20;     // ������ �������� ���� � ������
const int FIELD_X = 30;          // X-���������� ������ �������� ���� �������� ����
const int FIELD_Y = 30;          // Y-���������� ������ �������� ���� �������� ����
const int NEXT_X = 330;          // X-���������� ������� �������������
const int NEXT_Y = 30;           // Y-���������� ������� �������������

// ==============================================
// ���������� ����������
// ==============================================

// ��������� ������ �������� ���� (0 = �����, 1 = ������)
int field[FIELD_HEIGHT][FIELD_WIDTH];

// ������ � ������� ���� 7 ����� ���������
int tetrominos[7][4][4] = {
    // I - ������ �����
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
    // O - �������  
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
    // T - ������ � ��������
    {{0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0}},
    // S - ������ �����
    {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
    // Z - ������ ������
    {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
    // J - ����� J
    {{0,0,0,0},{1,0,0,0},{1,1,1,0},{0,0,0,0}},
    // L - ����� L
    {{0,0,0,0},{0,0,1,0},{1,1,1,0},{0,0,0,0}}
};

// ==============================================
// ��������� ������
// ==============================================

// ��������� ��� ������������� ������� ������
struct Piece {
    int type;     // ��� ������ (0-6)
    int x, y;     // ������� ������ �� ������� ����
};

// ��������� ��� ����� � ����������
struct GameStats {
    int score;           // ���� ������
    int lines;           // ���������� �������� �����
    int level;           // ������� ����
    int pieces;          // ���������� ������� �����
};

// ������� ���������������� ������
Piece currentPiece;
// ��������� ������ (������������ � ������� �������������)
Piece nextPiece;
// ���������� ����
GameStats stats;
// ����� ��� ����������� ������
sf::Font font;

// ==============================================
// �������
// ==============================================

/**
 * ������������� ����
 */
void initGame() {
    // ������� �� ������� ����
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            field[i][j] = 0;
        }
    }

    // ������� ���������������� ����� �� ���� (������ 3 ���� � ����������)
    for (int i = FIELD_HEIGHT - 3; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (j != 2 && j != 7) { // ��������� ��������
                field[i][j] = 1;
            }
        }
    }

    srand(time(nullptr));

    // ������ ���������������� ������
    currentPiece.type = rand() % 7;
    currentPiece.x = FIELD_WIDTH / 2 - 2;
    currentPiece.y = 5;

    nextPiece.type = rand() % 7;
    nextPiece.x = 0;
    nextPiece.y = 0;

    // �������������� ����������
    stats.score = 12340;      // ���������������� ����
    stats.lines = 15;         // ���������������� ���������� �����
    stats.level = 3;          // ���������������� �������
    stats.pieces = 47;        // ���������������� ���������� �����
}

/**
 * ������������� ������
 */
bool initFont() {
    // �������� ��������� ��������� ����� Windows
    return font.loadFromFile("C:/Windows/Fonts/arial.ttf");
}

/**
 * ��������� ����������� �����
 */
void drawBlock(sf::RenderWindow& window, int x, int y) {
    // ���� 1: ����� ������� ������� 28x28
    sf::RectangleShape outerBlock(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    outerBlock.setPosition(x, y);
    outerBlock.setFillColor(sf::Color::White);
    window.draw(outerBlock);

    // ���� 2: ׸���� ������� ������� 26x26
    sf::RectangleShape middleBlock(sf::Vector2f(26, 26));
    middleBlock.setPosition(x + 1, y + 1);
    middleBlock.setFillColor(sf::Color::Black);
    window.draw(middleBlock);

    // ���� 3: ����� ���������� ������� 24x24
    sf::RectangleShape innerBlock(sf::Vector2f(24, 24));
    innerBlock.setPosition(x + 2, y + 2);
    innerBlock.setFillColor(sf::Color::White);
    window.draw(innerBlock);
}

/**
 * ��������� ������
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
 * ����� ���������������� �����
 */
void changeDemoFigures() {
    currentPiece.type = rand() % 7;
    nextPiece.type = rand() % 7;

    // ����������� ���������������� ����������
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
 * �������������� ����� � ������
 */
std::string toString(int number) {
    std::stringstream ss;
    ss << number;
    return ss.str();
}

// ==============================================
// �������� �������
// ==============================================

int main() {
    // ������ ����
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris - Visual Demo");

    // �������������� ����
    initGame();

    // �������� ��������� �����
    bool fontLoaded = initFont();

    // �������� ���� �����������
    while (window.isOpen()) {
        // ��������� �������
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Space:
                    // ������ - ����� ���������������� �����
                    changeDemoFigures();
                    break;
                case sf::Keyboard::Escape:
                    // Escape - �����
                    window.close();
                    break;
                default:
                    break;
                }
            }
        }

        // ���������
        window.clear(sf::Color::Black);

        // ������ ������� ��������� �������� ����
        sf::RectangleShape fieldBorder(sf::Vector2f(280, 560));
        fieldBorder.setPosition(FIELD_X, FIELD_Y);
        fieldBorder.setFillColor(sf::Color::Transparent);
        fieldBorder.setOutlineThickness(2);
        fieldBorder.setOutlineColor(sf::Color::White);
        window.draw(fieldBorder);

        // ������ ������� ������� �������������
        sf::RectangleShape nextBorder(sf::Vector2f(126, 126));
        nextBorder.setPosition(NEXT_X, NEXT_Y);
        nextBorder.setFillColor(sf::Color::Transparent);
        nextBorder.setOutlineThickness(2);
        nextBorder.setOutlineColor(sf::Color::White);
        window.draw(nextBorder);

        // ������ ����������� ����� �� ������� ����
        for (int i = 0; i < FIELD_HEIGHT; i++) {
            for (int j = 0; j < FIELD_WIDTH; j++) {
                if (field[i][j]) {
                    int blockX = FIELD_X + j * BLOCK_SIZE;
                    int blockY = FIELD_Y + i * BLOCK_SIZE;
                    drawBlock(window, blockX, blockY);
                }
            }
        }

        // ������ ������� ���������������� ������
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (tetrominos[currentPiece.type][i][j]) {
                    int blockX = FIELD_X + (currentPiece.x + j) * BLOCK_SIZE;
                    int blockY = FIELD_Y + (currentPiece.y + i) * BLOCK_SIZE;
                    drawBlock(window, blockX, blockY);
                }
            }
        }

        // ������ ��������� ������ � ������� �������������
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (tetrominos[nextPiece.type][i][j]) {
                    int blockX = NEXT_X + 20 + j * BLOCK_SIZE;
                    int blockY = NEXT_Y + 20 + i * BLOCK_SIZE;
                    drawBlock(window, blockX, blockY);
                }
            }
        }

        // ��������� ���������� � �����
        if (fontLoaded) {
            // ��������� "Next"
            drawText(window, "NEXT", NEXT_X + 35, NEXT_Y - 25, 16, sf::Color::White);

            // ���������� ������ �� ������� Next
            int statsX = NEXT_X;
            int statsY = NEXT_Y + 140;

            // ����
            drawText(window, "SCORE", statsX, statsY, 14, sf::Color::Yellow);
            drawText(window, toString(stats.score), statsX, statsY + 20, 16, sf::Color::White);

            // �����
            drawText(window, "LINES", statsX, statsY + 50, 14, sf::Color::Yellow);
            drawText(window, toString(stats.lines), statsX, statsY + 70, 16, sf::Color::White);

            // �������
            drawText(window, "LEVEL", statsX, statsY + 100, 14, sf::Color::Yellow);
            drawText(window, toString(stats.level), statsX, statsY + 120, 16, sf::Color::White);

            // ���������� �����
            drawText(window, "PIECES", statsX, statsY + 150, 14, sf::Color::Yellow);
            drawText(window, toString(stats.pieces), statsX, statsY + 170, 16, sf::Color::White);

            // ���������� �����
            drawText(window, "SPACE - Change Figures", 50, WINDOW_HEIGHT - 60, 12, sf::Color::Cyan);
            drawText(window, "ESC - Exit", 50, WINDOW_HEIGHT - 40, 12, sf::Color::Cyan);
        }
        else {
            // ���� ����� �� ����������, ���������� ����� ��� ����������
            sf::RectangleShape scoreBg(sf::Vector2f(100, 200));
            scoreBg.setPosition(NEXT_X, NEXT_Y + 140);
            scoreBg.setFillColor(sf::Color(50, 50, 50));
            scoreBg.setOutlineThickness(1);
            scoreBg.setOutlineColor(sf::Color::White);
            window.draw(scoreBg);
        }

        // ���������� �� ������������ �� ������
        window.display();
    }

    return 0;
}