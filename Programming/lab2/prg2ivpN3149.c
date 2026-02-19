#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define ull unsigned long long

//Глобальные переменные
ull N;
ull M;
double *matr;
char *DEBUG;

//Для удобного взаимодействия с матрицей
#define matrix(a, b) *get_elem(a, b)
double *get_elem(ull n, ull m){
    return matr + n*M + m;
}

void print(){
    for (ull i = 0; i < N; i++){
        for (ull j = 0; j < M; j++) printf("%f ", matrix(i, j));
        printf("\n");
    }
}

//проверка на валидность
int is_valid_double(char *str){
    ull len = strlen(str);
    int dot = 0;
    for (ull i = 0; i < len; i++){
        if (i == 0 && str[i] == '-' && len > 1) continue;
        if (str[i] >= '0' && str[i] <= '9') continue;
        if (str[i] == '.' && dot == 0){dot++; continue;}
        return 0;
    }
    return 1;
}

int is_valid_ull(char *param){
    int res;
    ull num;
    res = sscanf(param, "%llu", &num);
    if (!res) return 0;

    unsigned long int znaki = 0;
    while (num > 0){
        num /= 10;
        znaki++;
    }
    if (znaki != strlen(param)) return 0;

    return 1;
}

//удаление строки или столбца
void del_strok(ull n){
    for (ull i = n; i < N; i++){
        for (ull j = 0; j < M; j++) matrix(i, j) = matrix(i+1, j);
    }
    matr = realloc(matr, (--N)*M*sizeof(*matr));
    if (N == 0) M = 0;
}

void del_stolb(ull m){
    ull k = 0;
    for (ull i = 0; i < N*(M-1); i++){
        if (i + k == k*M + m) k++;
        *(matr + i) = *(matr + i + k);
    }
    matr = realloc(matr, N*(--M)*sizeof(*matr));
    if (M == 0) N = 0;
}

//подсчёт характеристики по сумме в строке или столбце
ull character(long double n){
    if (n < 0) n = -n; // так как корень можно брать только из положительного, я беру модуль числа
    ull res = 0;
    for (; res*res <= n; res++){}
    return res - 1;
}

//преобразование в соответствии с заданием
void transformation(){
    ull ch_strok[N];
    ull ch_stolb[M];
    long double sum;
    start:
    for (ull i = 0; i < N; i++){
        sum = 0;
        for (ull j = 0; j < M; j++) sum += matrix(i, j);
        ch_strok[i] = character(sum);
        for (ull j = 0; j < i; j++){
            if (ch_strok[i] == ch_strok[j]){
                del_strok(i);
                if (DEBUG){
                    fprintf(stderr, "Характеристики строк %llu и %llu совпали и равны %llu, удаление строки %llu\n", j+1, i+1, ch_strok[i], i+1);
                    print();
                }
                goto start;
            }
        }
    }
    for (ull j = 0; j < M; j++) {
        sum = 0;
        for (ull i = 0; i < N; i++) sum += matrix(i, j);
        ch_stolb[j] = character(sum);
        for (ull i = 0; i < N; i++){
            if (ch_strok[i] == ch_stolb[j]){
                del_stolb(j);
                if (DEBUG){
                    fprintf(stderr, "Характеристики столбца %llu и стороки %llu совпали и равны %llu, удаление столбца %llu\n", j+1, i+1, ch_strok[i], j+1);
                    print();
                }
                goto start;
            }
        }
        for (ull i = 0; i < j; i++){
            if (ch_stolb[i] == ch_stolb[j]){
                del_stolb(j);
                if (DEBUG){
                    fprintf(stderr, "Характеристики столбцов %llu и %llu совпали и равны %llu, удаление столбца %llu\n", j+1, i+1, ch_stolb[i], j+1);
                    print();
                }
                goto start;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    // Проверка запуска с переменной среды, включающей отладочный вывод.
    // Пример запуска с установкой переменной LAB2DEBUG в 1:
    // $ LAB2DEBUG=1 ./prg2abcNXXXXX 5 5
    DEBUG = getenv("LAB2DEBUG");
    int first_param = 1;

    //проверка переданных параметров
    {
    if (DEBUG) {
        fprintf(stderr, "Включен вывод отладочных сообщений\n");
    }
    if (argc == 1){
        fprintf(stderr, "Usage: %s [-m] число_строк число_столбцов\n", argv[0]);
        return EXIT_SUCCESS;
    }
    if ((argc > 4) || (argc < 3) || (argc == 4 && (argv[1][0] != '-'))){
        fprintf(stderr, "Ошибка: неверное число параметров\nUsage: %s [-m] число_строк число_столбцов\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "-m") && (argv[1][0] == '-') && (strlen(argv[1]) > 1)){
        fprintf(stderr, "Ошибка: опция \'%s\' не поддерживается\n", argv[1]);
        return EXIT_FAILURE;
    }
    if (argc == 4) first_param++;
    for (int i = 0; i < 2; i++){
        if (!is_valid_ull(argv[first_param])){
            fprintf(stderr, "Ошибка: \'%s\' не является целым положительным числом\n", argv[first_param]);
            return EXIT_FAILURE;
        }
        first_param++;
    }
    }


    N = strtoull(argv[first_param-2], NULL, 10);
    M = strtoull(argv[first_param-1], NULL, 10);
    matr = calloc(N*M, sizeof(*matr));
    if (!matr){
        fprintf(stderr, "Ошибка: Недостаточно памяти\nПопробуйте ввести числа поменьше\n");
        return EXIT_FAILURE;
    }

    
    if (!strcmp(argv[1], "-m")){
        char line[sizeof(double)*8/3*2]; //2^3=8, окуглим до 10, тода  нас не более sizeof(double)*8/3 знаков, умножаем на 2, чтоб с запасом было(т.к. есть - и .)
        for (ull i = 0; i < N*M; i++){
            scanf("%s", line);
            if (!is_valid_double(line)){
                fprintf(stderr, "Ошибка: \'%s\' не double\n", line);
                return EXIT_FAILURE;
            }
            sscanf(line, "%lf", matr+i);
        }
    }
    else{
        srand(time(NULL) + getpid());
        for (ull i = 0; i < N; i++){
            for (ull j = 0; j < M; j++) matrix(i, j) = (2*(rand()%2) - 1)*((double)rand() + ((double)rand() / RAND_MAX));
        }
    }
    //защита от ./prg2ivpN3149 '' 1
    if (N*M == 0){
        fprintf(stderr, "Пожалуйста, не используйте ковычки\n");
        return EXIT_FAILURE;
    }

    printf("Исходная матрица:\n");
    print();

    transformation();

    printf("\nРезультат:\n");
    print();

    if (DEBUG){ 
        long double sum;
        fprintf(stderr, "Характеристики строк:\n");
        for (ull i = 0; i < N; i++){
            sum = 0;
            for (ull j = 0; j < M; j++) sum += matrix(i, j);
            fprintf(stderr, "%llu ", character(sum));
        }
        fprintf(stderr, "\nХарактеристики столбцов:\n");
        for (ull j = 0; j < M; j++) {
            sum = 0;
            for (ull i = 0; i < N; i++) sum += matrix(i, j);
            fprintf(stderr, "%llu ", character(sum));
        }
        fprintf(stderr, "\n");
    }
    free(matr);
    return EXIT_SUCCESS;
}