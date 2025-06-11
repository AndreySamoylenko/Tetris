#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
using namespace std;
// добавить конекты между функциями
int map[10][20];

/*тетромино первая координата горизонтальная вторая вертикальна*/

/*консольный вывод поля // для тестов*/

void out(int map[10][20]){
    for (int i = 0; i < 20; i++){
        cout << '|';
        for( int j = 0; j < 10; j++){   
            if(map[j][i])
            {
                cout << "[#]";
            }
            else cout << " . ";


        }
        cout << '|';
        cout << endl;
    }
    cout << " ============================== " << endl;

}

/*очищает нижние слои от рядов тетромино*/

void clear(int (&map)[10][20]){
    int k = 0;
    for (int i = 19; i >= 0; i--){
        if( map[0][i] && map[1][i] && map[2][i] && map[3][i] && map[4][i] && map[5][i] && map[6][i] && map[7][i] && map[8][i] && map[9][i]){
            k++;
            for (int j = 0; j < 10; j++){
                map[j][i] = 0;
            }
        }
        else{
            for (int j = 0; j < 10; j++){
                map[j][i+k] = map[j][i];
                map[j][i] = 0;
            }
        }
    }


}

/*опускает текущую тетрорминошку на поле map на одну клетку, tetromino - координаты тетроминошки поблочно*/

bool fall(int (&map)[10][20], int (&tetramino)[4][2]){
    if(!(map[tetramino[0][0]][tetramino[0][1] + 1] || map[tetramino[1][0]][tetramino[1][1] + 1] ||
       map[tetramino[2][0]][tetramino[2][1] + 1] || map[tetramino[3][0]][tetramino[3][1] + 1] ||
       tetramino[0][1] == 19 || tetramino[1][1] == 19 || tetramino[2][1] == 19 || tetramino[3][1] == 19))
    {
        for (int i = 0; i < 4; i++){
        int a = tetramino[i][0];
        int b = tetramino[i][1];
        map[a][b] = 0;
        }    
        for (int i = 0; i < 4; i++){
        ++tetramino[i][1];
        int a = tetramino[i][0];
        int b = tetramino[i][1];
        map[a][b] = 1;
        }
        return true;
    }
    else return false;
}

/*выбор случайной тетроминошки адекватым образом + можно отслеживать следущую тетроминошку(лубую следущую из 13 (дальше порядок ролится заново))*/

int choose_teromino(){
    static int tetromino_list[14] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6};
    static int curent_tetromino_pos = 13;


    if (curent_tetromino_pos == 19){
        shuffle(begin(tetromino_list), end(tetromino_list), mt19937{random_device{}()});
        curent_tetromino_pos = 0;
    }
    else{
        curent_tetromino_pos++;
    }
    return tetromino_list[curent_tetromino_pos];
}

/* Добовляет на поле map тетроминошку соответстующего типа, записывая её координаты(поблочно) в tetromino*/

bool add_tetromino(int (&map)[10][20], int tetromino_type, int (&tetromino)[4][2]){
    switch (tetromino_type)
    {
    case 0: 
    //[#][#]
    //[#][#]
        if (!(map[4][0] || map[5][0] || map[4][1] || map[5][1])){
            map[4][0] = 1;
            map[5][0] = 1;
            map[4][1] = 1;
            map[5][1] = 1;

            for (int i = 0; i < 2; i++){
                tetromino[i][0] = 4 + i;
                tetromino[i][1] = 0;
            }
            for (int i = 2; i < 4; i++){
                tetromino[i][0] = 2 + i;
                tetromino[i][1] = 1;
            } 
            return 1;
        }
        else return 0;
        break;
    case 1: 
    // [#][#][#][#]
        if( !(map[3][0] || map[4][0] || map[5][0] || map[6][0]) )
        {
        map[3][0] = 1;
        map[4][0] = 1;
        map[5][0] = 1;
        map[6][0] = 1;

        for (int i = 0; i < 4; i++){
            tetromino[i][0] = 3 + i;
            tetromino[i][1] = 0;
        }
        return 1;
        }
        else return 0;
        break;
    case 2:
    /* [#]
       [#][#][#] */
        if(!(map[3][0] || map[4][0] || map[5][0] || map[5][1])){
            map[3][0] = 1;
            map[4][0] = 1;
            map[5][0] = 1;
            map[5][1] = 1;

            for (int i = 0; i < 3; i++){
            tetromino[i][0] = 3 + i;
            tetromino[i][1] = 0;
            }
            tetromino[3][0] = 5; tetromino[3][1] = 1;
            return 1;
        }
        else return 0;

        break;
    case 3:
    /* [#][#][#]
       [#]       */
       if(!(map[3][0] || map[4][0] || map[5][0] || map[3][1])){
            map[3][0] = 1;
            map[4][0] = 1;
            map[5][0] = 1;
            map[3][1] = 1;

            for (int i = 0; i < 3; i++){
            tetromino[i][0] = 3 + i;
            tetromino[i][1] = 0;
            }
            tetromino[3][0] = 3; tetromino[3][1] = 1;
            return 1;
        }
        else return 0;
        break;
    case 4:
    /* [#][#]
          [#][#] */
        if (!(map[3][0] || map[4][0] || map[4][1] || map[5][1])){
            map[3][0] = 1;
            map[4][0] = 1;
            map[4][1] = 1;
            map[5][1] = 1;

            for (int i = 0; i < 2; i++){
                tetromino[i][0] = 0;
                tetromino[i][1] = 3 + i;
            }
            for (int i = 2; i < 4; i++){
                tetromino[i][0] = 1;
                tetromino[i][1] = 2 + i;
            } 
            return 1;
        }
        else return 0;
        break;
    case 5:
    /*    [#][#]
       [#][#]     */
        if (!(map[5][0] || map[4][0] || map[4][1] || map[3][1])){
            map[5][0] = 1;
            map[4][0] = 1;
            map[4][1] = 1;
            map[3][1] = 1;

            for (int i = 0; i < 2; i++){
                tetromino[i][0] = 4 + i;
                tetromino[i][1] = 0;
            }
            for (int i = 2; i < 4; i++){
                tetromino[i][0] = 1 + i;
                tetromino[i][1] = 0;
            } 
            return 1;
        }
        else return 0;
        break;
    case 6:
    /* [#][#][#]
          [#]     */
        if(!(map[3][0] || map[4][0] || map[5][0] || map[4][1])){
            map[3][0] = 1;
            map[4][0] = 1;
            map[5][0] = 1;
            map[4][1] = 1;

            for (int i = 0; i < 3; i++){
            tetromino[i][0] = 3 + i;
            tetromino[i][1] = 0;
            }
            tetromino[3][0] = 4; tetromino[3][1] = 1;
            return 1;
        }
        else return 0; 
        break;
    
    default:
        return 0;
        break;
    }
}

/* двигает тетроминошку базово вправо, при добавлении 3 параметра отличного от 0 влево*/

bool move(int (&map)[10][20], int (&tetromino)[4][2], bool direction = 0){
    if(direction){
        for (int i = 0; i < 4; i++) map[tetromino[i][0]][tetromino[i][1]] = 0;
        if (!(
            tetromino[0][0] < 1 || tetromino[1][0] < 1 || tetromino[2][0] < 1 || tetromino[3][0] < 1
        )){
            if (!(map[tetromino[0][0]-1][tetromino[0][1]] || map[tetromino[1][0]-1][tetromino[1][1]] || map[tetromino[2][0]-1][tetromino[2][1]] || map[tetromino[3][0]-1][tetromino[3][1]])){
                for (int i = 0; i < 4; i++) tetromino[i][0]--;
                for (int i = 0; i < 4; i++){
                    map[tetromino[i][0]][tetromino[i][1]] = 1;
                }
                return 1;
            }
        }
        for (int i = 0; i < 4; i++){
            map[tetromino[i][0]][tetromino[i][1]] = 1;
        }
        return 0;
    }
    else{
        for (int i = 0; i < 4; i++) map[tetromino[i][0]][tetromino[i][1]] = 0;
        if (!(
            tetromino[0][0] >= 9 || tetromino[1][0] >= 9 || tetromino[2][0] >= 9 || tetromino[3][0] >= 9
        )){
            if (!(map[tetromino[0][0]+1][tetromino[0][1]] || map[tetromino[1][0]+1][tetromino[1][1]] || map[tetromino[2][0]+1][tetromino[2][1]] || map[tetromino[3][0]+1][tetromino[3][1]])){
                for (int i = 0; i < 4; i++) tetromino[i][0]++;
                for (int i = 0; i < 4; i++){
                    map[tetromino[i][0]][tetromino[i][1]] = 1;
                }
                return 1;
            }
        }
        for (int i = 0; i < 4; i++){
            map[tetromino[i][0]][tetromino[i][1]] = 1;
        }
        return 0;
    }
}

bool rotate();


int main(){
    for (int i = 0; i < 20; i++){
        for( int j = 0; j < 10; j++){
            map[i][j] = 0;
        }
    }
    out(map);
    for (int i = 0; i < 10; i++) map[19][i] = 1;
    for (int i = 0; i < 10; i++) map[18][i] = 1;
    int in = 90;
    int tetromino[4][2];

}