#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  //  Для sleep
#define HEIGHT 80
#define WIDTH 25

void matrix_drawing(int **a);  // рисование матриц
void matrix_movement(int **a, int **a1);
// копирование из матрицы следующего положения в матрицу текущего
int matrix_comparison(int **a, int **a1);
// сравнение матрицы текущего положения и следующего положения
int game_stop(int **a, int **a1);
// функция остановки игры
void array_transformation(int **a, int **a1);
// превращение пред-матрицы в матрицу
void matrix_check(int **a1, int **a);
// проверка на количество живых клеток около каждого элемента матрицы
int middle_check(int **a, int i, int j);
// проверка клетки, которая находится не на границе
int check_i_start(int **a, int i, int j);
// проверка клетки на границе сверху
int check_i_end(int **a, int i, int j);
// проверка клетки на границе снизу
int check_j_start(int **a, int i, int j);
// проверка клетки на границе слева
int check_j_end(int **a, int i, int j);
// проверка клетки на границе справа
int check_i_start_j_start(int **a, int i, int j);
// проверка клетки в левом верхнем углу
int check_i_end_j_start(int **a, int i, int j);
// проверка клетки в левом нижнем углу
int check_i_start_j_end(int **a, int i, int j);
// проверка клетки в правом верхнем углу
int check_i_end_j_end(int **a, int i, int j);
// проверка клетки в правом нижнем углу
void read_function(int **a);  // чтение из файла
int speed();
int main() {
    int** matrix1 = malloc(WIDTH * sizeof(int*));  // матрица текущего положения
    for (int i = 0; i < WIDTH; ++i) {  // выделение динамической памяти
        matrix1[i] = malloc(HEIGHT * sizeof(int));
    }
    int** matrix2 = malloc(WIDTH * sizeof(int*));
    //  матрица следующего положения
    for (int i = 0; i < WIDTH; ++i) {   // выделение динамической памяти
        matrix2[i] = malloc(HEIGHT * sizeof(int));
    }
    int** matrix3 = malloc(WIDTH * sizeof(int*));
    // матрица для сохранения предыдущего результата
    for (int i = 0; i < WIDTH; ++i) {  // выделение динамической памяти
        matrix3[i] = malloc(HEIGHT * sizeof(int));
    }
    read_function(matrix1);
    int w = 0;
    w = speed();
    int n = 0;
    while ((game_stop(matrix3, matrix2) == 1 ||
            game_stop(matrix2, matrix1) == 1) && n != 10000) {
        // проверка условий выхода между матрицами
        printf("\033c");
        printf("\n");
        matrix_drawing(matrix1);  //  рисование матрицы
        printf("\n");
        matrix_check(matrix1, matrix2);
        //  записывает в каждый эемент массива колличество соседей
        array_transformation(matrix2, matrix1);
        //  записывает в каждый элемент 0 или 1
        matrix_movement(matrix3, matrix1);
        //  перемещение матрицы
        matrix_movement(matrix1, matrix2);
        //  перемещение матрицы
        n++;
        printf("\n");
        usleep(w);
    }
    for (int i = 0; i < n; i++) {
        free(matrix1[i]);
    }
    for (int i = 0; i < n; i++) {
        free(matrix2[i]);
    }
    for (int i = 0; i < n; i++) {
        free(matrix3[i]);
    }
    return 0;
}
int speed() {
    int y;
    int x;
    printf("Введите номер скорости. 1 - 1000 2 - 10000 3 - 100000\n");
    scanf("%d", &y);
        switch (y) {
            case 1:
                x = 1000;
                break;
            case 2:
                x = 10000;
                break;
            case 3:
                x = 100000;
                break;
            default:
                printf("Вы ввели неправильное значение. Запустите игру заново");
                exit(1);
        }
    return x;
}
void matrix_drawing(int **a) {  //  поэлементное рисование матрицы
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (a[i][j] == 0) {
                printf(" ");
            } else {
                printf("o");
            }
        }
        printf("\n");
    }
}
void matrix_movement(int **a, int **a1) {
    //  перемещение из одной матрицы в другую поэлементно
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            a[i][j] = a1[i][j];
        }
    }
}

int matrix_comparison(int **a, int **a1) {  //  поэлементно сравнивает матрицы
    int comparison_flag = 0;
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (a[i][j] != a1[i][j]) {
                comparison_flag++;
            }
        }
    }
    if (comparison_flag != 0) {
        return 1;  //  возвращает 1, если матрицы не равны
    } else {
        return 0;  //  возращает 0, если матрицы равны
    }
}

int game_stop(int **a, int **a1) {
    int void_flag = 0;
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (a1[i][j] == 1) {
                void_flag++;
            }
        }
    }
    if (matrix_comparison(a, a1) == 1) {
        void_flag++;
    }
    if (void_flag > 0) {
        return 1;  //  игра продолжается дальше
    } else {
        return 0;  //  игра заканчивается
    }
}
void array_transformation(int **a, int **a1) {
    // матрица с 1, 2, 3, 0 превращается в матрицу с 0 и 1
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (a[i][j] == 3 && a1[i][j] == 0) {
                a[i][j] = 1;
            } else if (a[i][j] == 2 && a1[i][j] == 1) {
                a[i][j] = 1;
            } else if (a[i][j] == 3 && a1[i][j] == 1) {
                a[i][j] = 1;
            } else if (a[i][j] == 2 && a1[i][j] == 0) {
                a[i][j] = 0;
            } else if (a[i][j] > 3 && a1[i][j] == 0) {
                a[i][j] = 0;
            } else if (a[i][j] >= 3 && a1[i][j] == 1) {
                a[i][j] = 0;
            } else if (a[i][j] < 2) {
                a[i][j] = 0;
            }
        }
    }
}

void matrix_check(int **a1, int **a) {
    // проверка на количество живых клеток около каждого элемента матрицы
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (i == WIDTH-1 && j == HEIGHT-1) {
                a[i][j] = check_i_end_j_end(a1, i, j);
            } else if (i == 0 && j == HEIGHT-1) {
                a[i][j] = check_i_start_j_end(a1, i, j);
            } else if (i == WIDTH-1 && j == 0) {
                a[i][j] = check_i_end_j_start(a1, i, j);
            } else if (i == 0 && j == 0) {
                a[i][j] = check_i_start_j_start(a1, i, j);
            } else if (j == HEIGHT-1) {
                a[i][j] = check_j_end(a1, i, j);
            } else if (j == 0) {
                a[i][j] = check_j_start(a1, i, j);
            } else if (i == WIDTH-1) {
                a[i][j] = check_i_end(a1, i, j);
            } else if (i == 0) {
                a[i][j] = check_i_start(a1, i, j);
            } else if (i != 0 && i != WIDTH-1 && j != 0 && j != HEIGHT-1) {
                a[i][j] = middle_check(a1, i, j);
            }
        }
    }
}

int middle_check(int **a, int i, int j) {
    int quantity_flag = 0;  //  флаг количества живых клеток
    if (a[i][j+1] == 1) {
        quantity_flag++;
    }
    if (a[i][j-1] == 1) {
        quantity_flag++;
    }
    if (a[i-1][j] == 1) {
        quantity_flag++;
    }
    if (a[i+1][j] == 1) {
        quantity_flag++;
    }
    if (a[i-1][j-1] == 1) {
        quantity_flag++;
    }
    if (a[i-1][j+1] == 1) {
        quantity_flag++;
    }
    if (a[i+1][j-1] == 1) {
        quantity_flag++;
    }
    if (a[i+1][j+1] == 1) {
        quantity_flag++;
    }
    return quantity_flag;  //  возвращаем количесво живых клеток
}

int check_i_start(int **a, int i, int j) {
    int quantity_flag = 0;  // флаг количества живых клеток
    if (a[i][j+1] == 1) {
        quantity_flag++;
    }
    if (a[i][j-1] == 1) {
        quantity_flag++;
    }
    if (a[i+1][j] == 1) {
        quantity_flag++;
    }
    if (a[WIDTH-1][j] == 1) {
        quantity_flag++;
    }
    if (a[i+1][j+1] == 1) {
        quantity_flag++;
    }
    if (a[i+1][j-1] == 1) {
        quantity_flag++;
    }
    if (a[WIDTH-1][j-1] == 1) {
        quantity_flag++;
    }
    if (a[WIDTH-1][j+1] == 1) {
        quantity_flag++;
    }
    return quantity_flag;  //  возвращаем количесво живых клеток
}
int check_i_end(int **a, int i, int j) {
    int quantity_flag = 0;
    if (a[i][j+1] == 1) {
        quantity_flag++;
    }
    if (a[i][j-1] == 1) {
        quantity_flag++;
    }
    if (a[i-1][j] == 1) {
        quantity_flag++;
    }
    if (a[0][j] == 1) {
        quantity_flag++;
    }
    if (a[i-1][j+1] == 1) {
        quantity_flag++;
    }
    if (a[i-1][j-1] == 1) {
        quantity_flag++;
    }
    if (a[0][j-1] == 1) {
        quantity_flag++;
    }
    if (a[0][j+1] == 1) {
        quantity_flag++;
    }
    return quantity_flag;
}

int check_j_start(int **a, int i, int j) {
    int quantity_flag = 0;
    if (a[i][j+1] == 1) {
        quantity_flag++;
    }
    if (a[i][HEIGHT-1] == 1) {
        quantity_flag++;
    }
    if (a[i+1][j] == 1) {
        quantity_flag++;
    }
    if (a[i-1][j] == 1) {
        quantity_flag++;
    }
    if (a[i-1][j+1] == 1) {
        quantity_flag++;
    }
    if (a[i-1][HEIGHT-1] == 1) {
        quantity_flag++;
    }
    if (a[i+1][j+1] == 1) {
        quantity_flag++;
    }
    if (a[i+1][HEIGHT-1] == 1) {
        quantity_flag++;
    }
    return quantity_flag;
}
int check_j_end(int **a, int i, int j) {
    int quantity_flag = 0;
    if (a[i+1][j] == 1) {
      quantity_flag++;
    }
    if (a[i-1][j] == 1) {
        quantity_flag++;
    }
    if (a[i][0] == 1) {
        quantity_flag++;
    }
    if (a[i][j-1] == 1) {
        quantity_flag++;
    }
    if (a[i-1][j-1] == 1) {
        quantity_flag++;
    }
    if (a[i-1][0] == 1) {
        quantity_flag++;
    }
    if (a[i+1][0] == 1) {
        quantity_flag++;
    }
    if (a[i+1][j-1] == 1) {
        quantity_flag++;
    }
    return quantity_flag;
}

int check_i_start_j_start(int **a, int i, int j) {
    int quantity_flag = 0;
    if (a[i][j+1] == 1) {
        quantity_flag++;
    }
    if (a[i][HEIGHT-1] == 1) {
        quantity_flag++;
    }
    if (a[i+1][j] == 1) {
        quantity_flag++;
    }
    if (a[WIDTH-1][j] == 1) {
        quantity_flag++;
    }
    if (a[i+1][j+1] == 1) {
        quantity_flag++;
    }
    if (a[i+1][HEIGHT-1] == 1) {
        quantity_flag++;
    }
    if (a[WIDTH-1][j+1] == 1) {
        quantity_flag++;
    }
    if (a[WIDTH-1][HEIGHT-1] == 1) {
        quantity_flag++;
    }
    return quantity_flag;
}
int check_i_end_j_start(int **a, int i, int j) {
    int quantity_flag = 0;
    if (a[i][j+1] == 1) {
        quantity_flag++;
    }
    if (a[i][HEIGHT-1] == 1) {
        quantity_flag++;
    }
    if (a[i-1][j] == 1) {
        quantity_flag++;
    }
    if (a[0][j] == 1) {
        quantity_flag++;
    }
    if (a[i-1][j+1] == 1) {
        quantity_flag++;
    }
    if (a[i-1][HEIGHT-1] == 1) {
        quantity_flag++;
    }
    if (a[0][HEIGHT-1] == 1) {
        quantity_flag++;
    }
    if (a[0][j+1] == 1) {
        quantity_flag++;
    }
    return quantity_flag;
    }
int check_i_start_j_end(int **a, int i, int j) {
    int quantity_flag = 0;
    if (a[i][0] == 1) {
        quantity_flag++;
    }
    if (a[i][j-1] == 1) {
        quantity_flag++;
    }
    if (a[i+1][j] == 1) {
        quantity_flag++;
    }
    if (a[WIDTH-1][j] == 1) {
        quantity_flag++;
    }
    if (a[i+1][j-1] == 1) {
        quantity_flag++;
    }
    if (a[i+1][0] == 1) {
        quantity_flag++;
    }
    if (a[WIDTH-1][j-1] == 1) {
        quantity_flag++;
    }
    if (a[WIDTH-1][0] == 1) {
        quantity_flag++;
    }
    return quantity_flag;
}
int check_i_end_j_end(int **a, int i, int j) {
    int quantity_flag = 0;
    if (a[i][j-1] == 1) {
        quantity_flag++;
    }
    if (a[i][0] == 1) {
        quantity_flag++;
    }
    if (a[i-1][j] == 1) {
        quantity_flag++;
    }
    if (a[0][j] == 1) {
        quantity_flag++;
    }
    if (a[i-1][j-1] == 1) {
        quantity_flag++;
    }
    if (a[i-1][0] == 1) {
        quantity_flag++;
    }
    if (a[0][j-1] == 1) {
        quantity_flag++;
    }
    if (a[0][0] == 1) {
        quantity_flag++;
    }
    return quantity_flag;
}
void read_function(int **a) {
    FILE *file;
    int choise_the_number_of_file;
    printf("Введите название фигуры\n");
    scanf("%d", &choise_the_number_of_file);
        file = fopen("test1.txt", "r");
        switch (choise_the_number_of_file) {
            case 1:
                file = fopen("test1.txt", "r");
                break;
            case 2:
                file = fopen("test2.txt", "r");
                break;
            case 3:
                file = fopen("test3.txt", "r");
                break;
            case 4:
                file = fopen("test4.txt", "r");
                break;
            case 5:
                file = fopen("test5.txt", "r");
                break;
            default:
                printf("Вы ввели неправильное значение. Запустите игру заново");
                exit(1);
}
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            fscanf(file, " %1d", &a[i][j]);
    }
}
    fclose(file);
}
