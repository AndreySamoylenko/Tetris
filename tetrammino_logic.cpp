#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
using namespace std;
// добавить конекты между функциями


/*тетромино первая координата горизонтальная вторая вертикальна*/


const int WIDTH = 10;
const int HEIGHT = 20;
int tetromino[4][2];
int tetromino_type;
int rotation = 0;
int map[WIDTH][HEIGHT] = {0};
/*консольный вывод поля // для тестов*/
// Вывод игрового поля
void out() {
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

void clear() {
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

bool add_tetromino(int type, int (&tetromino)[4][2],int rotation) {

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
            tetromino[1][0] = 5; tetromino[1][1] = 0;
            tetromino[2][0] = 6; tetromino[2][1] = 0;
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
            tetromino[1][0] = 6; tetromino[1][1] = 0;
            tetromino[2][0] = 4; tetromino[2][1] = 1;
            tetromino[3][0] = 5; tetromino[3][1] = 1;
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
    rotation = 0;
    
    return true;
}

/* двигает тетроминошку базово вправо, при добавлении 3 параметра отличного от 0 влево*/

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
bool rotate(int (&tetromino)[4][2], int type, int &rotation_state) {
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
    
    if (is_valid_position(new_pos)) {
        for (int i = 0; i < 4; i++) {
            tetromino[i][0] = new_pos[i][0];
            tetromino[i][1] = new_pos[i][1];
            map[tetromino[i][0]][tetromino[i][1]] = 1;
        }
        rotation_state = (rotation_state + 1) % 4;
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

int main(){
    for (int i = 0; i < 20; i++){
        for( int j = 0; j < 10; j++){
            map[i][j] = 0;
        }
    }
    out();
    for (int i = 0; i < 10; i++) map[19][i] = 1;
    for (int i = 0; i < 10; i++) map[18][i] = 1;
    int in = 90;

    while (in != 200)
    {
    cin >> in;
    
    switch(in)
    {
    case 1:
        fall(tetromino);
        out();
        break;
    case 2:
        move(tetromino, 1);
        break;
    case 3:
        move(tetromino, -1);
        break;
    case 4:
        rotate(tetromino, tetromino_type, rotation);
        break;
    case 5:
        tetromino_type = choose_teromino();
        add_tetromino(tetromino_type, tetromino, rotation);
        break;
    case 6:
        clear();
        break;
    default:
        break;
    }
    }

}