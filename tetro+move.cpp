#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <windows.h>
#include <cctype>
using namespace std;
// добавить конекты между функциями


/*тетромино первая координата горизонтальная вторая вертикальна*/


const int WIDTH = 10;
const int HEIGHT = 20;
int tetromino[4][2];
int tetromino_type = 0;
int map[WIDTH][HEIGHT] = {0};
int inputKey = 5;
int nextTetromino;
int tetrominos = 0;
int score = 0;
int level = 0;
int lines_total = 0;
bool canRotate = 1;

int getFallDelay(int level) {
    int delays[] = {800, 700, 600, 500, 400, 350, 300, 250, 200, 150, 100};
    return delays[min(level, 10)];
}

/*консольный вывод поля // для тестов*/
// Вывод игрового поля
void out() {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        cout << '|';
        for(int x = 0; x < WIDTH; x++) {   
            cout << (map[x][y] ? "[#]" : " . ");
        }
        cout << '|' << endl;
    }
    cout << " ============================== " << endl;
}

/*очищает нижние слои от рядов тетромино*/

int clear() {
    int lines_cleared = 0;
    
    for (int y = HEIGHT-1; y >= 0; y--) {
        bool line_full = true;
        for (int x = 0; x < WIDTH; x++) {
            if (!map[x][y]) {
                line_full = false;
                break;
            }
        }
        
        if (line_full) {
            lines_cleared++;
            for (int x = 0; x < WIDTH; x++) {
                map[x][y] = 0;
            }
        }
        else if (lines_cleared > 0) {
            for (int x = 0; x < WIDTH; x++) {
                map[x][y+lines_cleared] = map[x][y];
                map[x][y] = 0;
            }
        }
    }
    out();
    lines_total += lines_cleared;
    if (lines_cleared == 4) score += 400;
    return static_cast<int>(lines_cleared * lines_cleared * 100);
}

/*проверка возможности позиции*/
bool is_valid_position(int (&tetromino)[4][2]) {
    for (int i = 0; i < 4; i++) {
        int x = tetromino[i][0];
        int y = tetromino[i][1];
        
        if (x < 0 || x >= WIDTH || y >= HEIGHT) return false;
        if (y >= 0 && map[x][y]) return false;
    }
    return true;
}

/*опускает текущую тетрорминошку на поле map на одну клетку, tetromino - координаты тетроминошки поблочно*/

bool fall(int (&tetromino)[4][2]) {

    for (int i = 0; i < 4; i++) {
        int x = tetromino[i][0];
        int y = tetromino[i][1];
        if (y >= 0) map[x][y] = 0;
    }
    

    int new_pos[4][2];
    for (int i = 0; i < 4; i++) {
        new_pos[i][0] = tetromino[i][0];
        new_pos[i][1] = tetromino[i][1] + 1;
    }
    
    if (is_valid_position(new_pos)) {
        for (int i = 0; i < 4; i++) {
            tetromino[i][1]++;
            map[tetromino[i][0]][tetromino[i][1]] = 1;
        }
        out();
        return true;
    }
    else {

        for (int i = 0; i < 4; i++) {
            if (tetromino[i][1] >= 0) {
                map[tetromino[i][0]][tetromino[i][1]] = 1;
            }
        }
        return false;
    }
    
}

/*выбор случайной тетроминошки адекватым образом + можно отслеживать следущую тетроминошку(лубую следущую из 13 (дальше порядок ролится заново))*/

int choose_teromino(){
    static int tetromino_list[14] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6};
    static int curent_tetromino_pos = 13;


    if (curent_tetromino_pos == 13){
        shuffle(begin(tetromino_list), end(tetromino_list), mt19937{random_device{}()});
        curent_tetromino_pos = 0;
    }
    else{
        curent_tetromino_pos++;
    }
    return tetromino_list[curent_tetromino_pos];
}

/* Добовляет на поле map тетроминошку соответстующего типа, записывая её координаты(поблочно) в tetromino*/

bool add_tetromino(int type, int (&tetromino)[4][2]) {
    tetromino_type = type;
    for (int i = 0; i < 4; i++) {
        tetromino[i][0] = 0;
        tetromino[i][1] = 0;
    }
    

    switch (type) {
        case 0: // O
            tetromino[0][0] = 4; tetromino[0][1] = 0;
            tetromino[1][0] = 5; tetromino[1][1] = 0;
            tetromino[2][0] = 4; tetromino[2][1] = 1;
            tetromino[3][0] = 5; tetromino[3][1] = 1;
            break;
        case 1: // I
            tetromino[0][0] = 3; tetromino[0][1] = 0;
            tetromino[1][0] = 4; tetromino[1][1] = 0;
            tetromino[2][0] = 5; tetromino[2][1] = 0;
            tetromino[3][0] = 6; tetromino[3][1] = 0;
            break;
        case 2: // L
            tetromino[0][0] = 4; tetromino[0][1] = 0;
            tetromino[1][0] = 6; tetromino[1][1] = 0;
            tetromino[2][0] = 5; tetromino[2][1] = 0;
            tetromino[3][0] = 6; tetromino[3][1] = 1;
            break;
        case 3: // J
            tetromino[0][0] = 4; tetromino[0][1] = 0;
            tetromino[1][0] = 5; tetromino[1][1] = 0;
            tetromino[2][0] = 6; tetromino[2][1] = 0;
            tetromino[3][0] = 4; tetromino[3][1] = 1;
            break;
        case 4: // S
            tetromino[0][0] = 5; tetromino[0][1] = 0;
            tetromino[1][0] = 5; tetromino[1][1] = 1;
            tetromino[2][0] = 4; tetromino[2][1] = 1;
            tetromino[3][0] = 6; tetromino[3][1] = 0;
            break;
        case 5: // Z
            tetromino[0][0] = 4; tetromino[0][1] = 0;
            tetromino[1][0] = 5; tetromino[1][1] = 0;
            tetromino[2][0] = 5; tetromino[2][1] = 1;
            tetromino[3][0] = 6; tetromino[3][1] = 1;
            break;
        case 6: // T
            tetromino[0][0] = 4; tetromino[0][1] = 0;
            tetromino[1][0] = 5; tetromino[1][1] = 0;
            tetromino[2][0] = 6; tetromino[2][1] = 0;
            tetromino[3][0] = 5; tetromino[3][1] = 1;
            break;
    }
    
    if (!is_valid_position(tetromino)) return false;
    

    for (int i = 0; i < 4; i++) {
        map[tetromino[i][0]][tetromino[i][1]] = 1;
    }
    out();
    tetrominos += 1;
    return true;
}

// Движение влево/вправо
bool move(int (&tetromino)[4][2], int dx) {

    for (int i = 0; i < 4; i++) {
        int x = tetromino[i][0];
        int y = tetromino[i][1];
        if (y >= 0) map[x][y] = 0;
    }
    

    int new_pos[4][2];
    for (int i = 0; i < 4; i++) {
        new_pos[i][0] = tetromino[i][0] + dx;
        new_pos[i][1] = tetromino[i][1];
    }
    
    if (is_valid_position(new_pos)) {
        for (int i = 0; i < 4; i++) {
            tetromino[i][0] += dx;
            map[tetromino[i][0]][tetromino[i][1]] = 1;
        }
        out();
        return true;
    }
    else {

        for (int i = 0; i < 4; i++) {
            if (tetromino[i][1] >= 0) {
                map[tetromino[i][0]][tetromino[i][1]] = 1;
            }
        }
        return false;
    }
}

// Поворот фигуры
bool rotate(int (&tetromino)[4][2], int type) {
    if (type == 0) return true; 
    for (int i = 0; i < 4; i++) {
        int x = tetromino[i][0];
        int y = tetromino[i][1];
        if (y >= 0) map[x][y] = 0;
    }

    int cx = tetromino[1][0];
    int cy = tetromino[1][1];

    int new_pos[4][2];
    for (int i = 0; i < 4; i++) {
        int dx = tetromino[i][0] - cx;
        int dy = tetromino[i][1] - cy;

        new_pos[i][0] = cx - dy;
        new_pos[i][1] = cy + dx;
    }

    for (int i = 0; i < 4; i++) {
        if (new_pos[i][0] < 0 || new_pos[i][0] >= WIDTH || new_pos[i][1] >= HEIGHT)
            return false;
    }

    int dx_try[] = {0, -1, 1, 0};
    int dy_try[] = {0, 0, 0, -1};

    for (int t = 0; t < 4; ++t) {
        int try_pos[4][2];
        bool out_of_top = false;
        for (int i = 0; i < 4; i++) {
            try_pos[i][0] = new_pos[i][0] + dx_try[t];
            try_pos[i][1] = new_pos[i][1] + dy_try[t];
            if (try_pos[i][1] < -1) out_of_top = true;
        }
        if (out_of_top) continue;

        if (is_valid_position(try_pos)) {
            for (int i = 0; i < 4; i++) {
                tetromino[i][0] = try_pos[i][0];
                tetromino[i][1] = try_pos[i][1];
                if (tetromino[i][1] >= 0)
                    map[tetromino[i][0]][tetromino[i][1]] = 1;
            }
            out();
            return true;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (tetromino[i][1] >= 0) {
            map[tetromino[i][0]][tetromino[i][1]] = 1;
        }
    }
    return false;
}

// Объединение логики
bool inputs(int intput){
    switch (intput){
        case 1:
            return fall(tetromino);
            break;
        
        case 2:
            return move(tetromino, -1);
            break;

        case 3:
            return move(tetromino, 1);
            break;
        
        case 4:
            return rotate(tetromino, tetromino_type);
            break;
        default:
            return 1;
    }
}

//!!!! нужны нормальные

void updateInput() {
    
    while (true) {
        if (GetAsyncKeyState('W') & 0x8000) {
            if (canRotate) {
                inputKey = 4;
                canRotate = false;
            }
        } else {
            canRotate = true;
        }

        if (GetAsyncKeyState('A') & 0x8000) inputKey = 2;
        else if (GetAsyncKeyState('D') & 0x8000) inputKey = 3;
        else if (GetAsyncKeyState('S') & 0x8000) inputKey = 1;
        else if (GetAsyncKeyState(27) & 0x8000) inputKey = 0;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

long getTimeMs() {
    return static_cast<long>(clock()) * 1000 / CLOCKS_PER_SEC;
}


int procces(){


    thread inputThread(updateInput);
    inputThread.detach(); // Отвязываем, чтобы он работал параллельно

    long nextTime = getTimeMs();
    bool game = 1;
    nextTetromino = choose_teromino();
    add_tetromino(nextTetromino, tetromino);
    nextTetromino = choose_teromino();

    while (game)
    {
        inputs(inputKey);
        inputKey = 5;

        long now = getTimeMs();
        if (now >= nextTime) {
            nextTime = now + getFallDelay(level);

            if (fall(tetromino)) {}
            else {
                
                score += clear();
                level = lines_total / 10;

                if (add_tetromino(nextTetromino, tetromino)) {
                    nextTetromino = choose_teromino();
                } else {
                    game = 0;
                }
            }
        }

        this_thread::sleep_for(chrono::milliseconds(50));
        out();
    }

    return score;
}


int main(){
    // Создаём поток для обработки ввода
    srand(time(0));
    procces();
    system("cls");    
    cout << score;

    system("cls"); 
    out();
    cout << score;
    
    cin >> nextTetromino;
}
