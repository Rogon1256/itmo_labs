#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

#define ull unsigned long long
const ull odin = (ull)1;

ull pow2(short pokazatel){
    if (pokazatel == 0) return 1;
    return pow2(pokazatel-1) * 2;
}
ull pow16(short pokazatel){
    return pow2(4*pokazatel);
}

void printbin(ull n){
    for (short i = sizeof(n)*8-1; i >= 0; i--){
        printf("%d", ((n & (odin << i)) > 0 ? 1 : 0));
        if (i % 8 == 0) printf(" ");
    }
    printf("\n");
}

int is_valid(char *str, ull *result) {
    ull ULLMAX = 0;
    for (ull i = 0; i < sizeof(ULLMAX)*8; i++){
        ULLMAX += pow2(i);
    }
    ull value = 0;
    ull max_div10 = ULLMAX / 10;
    ull max_mod10 = ULLMAX % 10;

    while (*str) {
        if (!isdigit(*str)) {
            fprintf(stderr, "Вводите только цифры!!!\n");
            return 0;
        }

        if (value > max_div10 || (value == max_div10 && (ull)(*str - '0') > max_mod10)) {
            fprintf(stderr, "Слишком большое число!!!\n");
            return 0;
        }

        value = value * 10 + (*str - '0');
        str++;
    }

    *result = value;
    return 1;
}

int main(int argc, char *argv[])
{
    char *DEBUG = getenv("LAB1DEBUG");
    if (DEBUG) {
        fprintf(stderr, "Включен вывод отладочных сообщений\n");
    }

    ull n = 0;
    srand(time(NULL) + getpid());

    // Получение числа пользователя
    switch (argc)
    {
    case 1:
        n = 0;
        for (unsigned long i = 0; i < sizeof(n)*8; i++){ n += (rand()%2)*pow2(i); }
        printf("Дайте угадаю, ваше число - %Lu?\n\n", n);
        break;
    case 2:
        if (is_valid(argv[1], &n) == 1) break; // без флага -Wno-implicit-fallthrough выдаст Warning

    default:
        fprintf(stderr, "Usage: %s [неотрицательное целое число меньшее чем 2^64]\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Ваше число в двоичной системе счисления:\n");
    printbin(n);

    printf("\nСейчас тетрады вашего числа поменяются случайным образом. \nВы хотите видеть какие тетрады меняются?(y/something)\n");
    char ans;
    scanf("%c", &ans);

    unsigned short tetr_count = sizeof(n)*2;
    
    // Тут мы меняем местами 2е случайные тетрады
    short tetr1num, tetr2num;
    for (short i = 0; i < 512; i++){
        tetr1num = rand()%tetr_count;
        tetr2num = rand()%tetr_count;
        // Поменять местами тетрады - это поменять 4е подряд идущих бита
        for (short j = 0; j < 4; j++){
            if ((bool)(n & (odin << (tetr1num*4+j))) != (bool)(n & (odin << (tetr2num*4+j)))){
                n ^= odin << (tetr1num*4+j);
                n ^= odin << (tetr2num*4+j);
            }
        }
        if (ans == 'y' && tetr1num != tetr2num){
            printf("swap %hu %hu\n", (tetr1num < tetr2num ? tetr1num: tetr2num) + 1, (tetr1num < tetr2num ? tetr2num: tetr1num) + 1);
            printbin(n);
            
        }
    }
    printf("\n");
    printbin(n);
    return EXIT_SUCCESS;
}

