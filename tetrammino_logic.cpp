#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
using namespace std;

int map[10][20];

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


bool add_tetromino(int (&map)[10][20], int tetromino_type, int (&tetromino)[4][2]){
    switch (tetromino_type)
    {
    case 0:

        
        break;
    case 1:
        if( !(map[0][3] || map[0][4] || map[0][5] || map[0][6]) )
        {
        map[0][3] = 1;
        map[0][4] = 1;
        map[0][5] = 1;
        map[0][6] = 1;

        for (int i = 0; i < 4; i++){
            tetromino[i][0] = 0;
            tetromino[i][1] = 3 + i;
        }
        return true;
        }
        else return false;
        break;
    case 2:
        /* code */
        break;
    case 3:
        /* code */
        break;
    case 4:
        /* code */
        break;
    case 5:
        /* code */
        break;
    case 6:
        /* code */
        break;
    
    default:
        break;
    }
}


int main(){
    for (int i = 0; i < 20; i++){
        for( int j = 0; j < 10; j++){
            map[i][j] = 0;
        }
    }
    out(map);




    
    int tetramino[4][2] =  {{0, -1},
                            {1, -1},
                            {2, -1},
                            {3, -1}};


}