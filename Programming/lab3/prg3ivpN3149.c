#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ull unsigned long long

typedef enum{
    COLOR,
    MARKDOWN
} action;

typedef enum{
    NO, 
    ONLY_INPUT,
    INPUT_AND_OUTPUT
} files;

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


int main(int argc, char *argv[]) {
    
    action format = MARKDOWN;
    ull str_begin = 1;
    ull str_end = __UINT64_MAX__;
    int is_n_flag = 0;

    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-v") == 0){
            printf("Иван Вячеславович Порошин, гр. N3149\nВариант 1-1-1-6\n");
            return EXIT_SUCCESS;}
        else if (strcmp(argv[i], "-c") == 0) format = COLOR;
        else if (argv[i][0] == '-' && argv[i][1] == 'b'){
            if (argv[i][2] != '='){
                fprintf(stderr, "Ошибка: используйте -b=число\n");
                return EXIT_FAILURE;
            }
            if (is_valid_ull(argv[i]+3) == 0){
                fprintf(stderr, "Ошибка: используйте в -b число поменьше\n");
                return EXIT_FAILURE;
            }
            str_begin = strtoull(argv[i]+3, NULL, 10);
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'e'){
            if (argv[i][2] != '='){
                fprintf(stderr, "Ошибка: используйте -e=число\n");
                return EXIT_FAILURE;
            }
            if (is_valid_ull(argv[i]+3) == 0){
                fprintf(stderr, "Ошибка: используйте в -e число поменьше\n");
                return EXIT_FAILURE;
            }
            str_end = strtoull(argv[i]+3, NULL, 10);
        }
        else if (strcmp(argv[i], "-n") == 0) is_n_flag = 1;
        else if (argv[i][0] == '-'){
            fprintf(stderr, "Ошибка: неизвестный флаг\n");
            return EXIT_FAILURE;
        }
    }

    files is_files = NO;

    if (argc > 3 && (argv[argc-3][0] != '-')){
        fprintf(stderr, "Ошибка: слишком много аргументов\n");
        return EXIT_FAILURE;
    }
    else if (argc > 2 && (argv[argc-2][0] != '-') && (argv[argc-1][0] != '-')){
        is_files = INPUT_AND_OUTPUT;
    }
    else if (argc > 1 && (argv[argc-1][0] != '-')){
        is_files = ONLY_INPUT;
    }
    for (unsigned int i = 1; i < argc - is_files; i++){
        if (argv[i][0] != '-'){
            fprintf(stderr, "Ошибка: флаги после аргументов\n");
            return EXIT_FAILURE;
        }
    }

    
    char* text = calloc(1024, sizeof(char));
    if (text == NULL){
        fprintf(stderr, "Ошибка: мало места\n");
        return EXIT_FAILURE;
    }
    FILE *f = stdin;
    if (is_files != NO){
        f = fopen(is_files == ONLY_INPUT ? argv[argc-1] : argv[argc-2], "r");
        if (!f){
            fprintf(stderr, "Ошибка: входного файла не существует\n");
            return EXIT_FAILURE;
        }
    }
    ull i = 0;
    char c;
    do{
        c = getc(f);
        text[i] = c;
        i++;
        if (i == strlen(text)){
            text = realloc(text, i*2*sizeof(char));
            if (text == NULL){
                fprintf(stderr, "Ошибка: мало места или слишком большой текст\n");
                return EXIT_FAILURE;
            }
        }

    }while (c != EOF);
    text[i-1] = '\0';
    fclose(f);
    f = stdout;
    if (is_files == INPUT_AND_OUTPUT){
        f = fopen(argv[argc-1], "w");
    }
    ull current_line = 1;
    i = 0;
    for(; i < strlen(text); i++){
        if (text[i] == '\n'){
            current_line++;
            fputc('\n', f);
            continue;
        }
        if (current_line < str_begin || current_line > str_end){
            fputc(text[i], f);
            continue;
        }
        if ((i==0 ? 1 : !isdigit(text[i-1])) && isdigit(text[i]) && (i < 2 ? 1 : !(text[i-1] == '.' && isdigit(text[i-2])))){
            ull first_digit = i;
            ull local_current_line = current_line;
            ull num_len = 0;
            int num = 0;
            int is_first_zero = 0;
            ull j = 0;
            for (int l = 0; l < 3; l++){
                num_len = 0;
                num = 0;
                is_first_zero = 0;
                j = 0;
                for(; text[first_digit+j] != '.'; j++){
                    if (isdigit(text[first_digit+j])){
                        if (num_len == 0 && text[first_digit+j] == '0'){
                            is_first_zero = 1;
                        }
                        if ((num_len > 0 && is_first_zero) || num_len == 3){
                            goto ip_wrong;
                        }
                        num *= 10;
                        num += text[first_digit+j] - '0';
                        num_len++;
                    }
                    else if (text[first_digit+j] == '\n')
                    {
                        local_current_line++;
                        if (local_current_line > str_end || is_n_flag){
                            goto ip_wrong;
                        }
                    }
                    else{
                        goto ip_wrong;
                    }
                }
                if (num > 255 || num_len == 0) goto ip_wrong;
                first_digit += j+1;
            }
            num_len = 0;
            num = 0;
            is_first_zero = 0;
            j = 0;
            ull last_digit = 0;
            for(; ; j++){
                if (isdigit(text[first_digit+j])){
                    if (num_len == 0 && text[first_digit+j] == '0'){
                        is_first_zero = 1;
                    }
                    if ((num_len > 0 && is_first_zero) || num_len == 3){
                        goto ip_wrong;
                    }
                    num *= 10;
                    num += text[first_digit+j] - '0';
                    last_digit = first_digit+j;
                    num_len++;
                }
                else if (text[first_digit+j] == '\n')
                {
                    local_current_line++;
                    if (local_current_line > str_end  || is_n_flag){
                        break;
                    }
                }
                else{
                    break;
                }
            }
            if (num > 255 || num_len == 0) goto ip_wrong;
            if (text[first_digit+j] == '.' && isdigit(text[first_digit+j+1])) goto ip_wrong;

            if (format == MARKDOWN) fputc('*', f);
            else fputs("\e[36m", f);
            
            for (ull l = i; l < last_digit+1; l++){
                fputc(text[l], f);
            }

            if (format == MARKDOWN) fputc('*', f);
            else fputs("\e[m", f);
            i = last_digit;
            continue;
        }
        ip_wrong: putc(text[i], f);
    }
    fclose(f);
    return EXIT_SUCCESS;
}
