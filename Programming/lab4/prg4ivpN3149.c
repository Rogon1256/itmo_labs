#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define exit_code int

struct list
{
    char *email;
    struct list *next;
};
struct list* create_node(){
    struct list* node = malloc(sizeof(struct list));
    node->email = NULL;
    node->next = NULL;
    return node;
    // return malloc(sizeof(struct list));
}

// Установить почту email на позицию index
void set_email(struct list *head, unsigned short index, char *email){
    for (unsigned short i = 0; i < index; i++) 
        head = head->next;
    head->email = (char *)malloc(strlen(email) + 1);
    strcpy(head->email, email);
}
// Расширение листа до len элементов
void extension_list(struct list *head, unsigned short len){
    for (unsigned short current_len = 1; current_len < len; current_len++)
    {
        if (head->next == NULL){
            head->next = create_node();
        }
        head = head->next;
    }
}
// Проверка почты на корректность
int check_email(char *email){
    int dogPos = -1, dotPos = -1, len = strlen(email);
    for (int i = 0; i < len; i++) {
        char c = email[i];


        if (!isalnum(c) && c != '@' && c != '.' && c != '_' && c != '-') return 0;
        if (c == '.') dotPos = i;
        if (c == '@') {
            if (dogPos != -1) return 0;
            dogPos = i;
        }
    }
    if (dogPos < 1 || dogPos == len - 1 || dotPos < dogPos + 2 || dotPos == len - 1) return 0;
    return 1; 
}

// Различные команды
void push_back(struct list *head, struct list *node){
    if (head->email == NULL){
        head->email = (char *)malloc(strlen(node->email) + 1);
        strcpy(head->email, node->email);
        return;
    }
    while (head->next != NULL){
        head = head->next;
    }
    head->next = node;
}
void pop_back(struct list *head){
    if (head->next == NULL) head->email = NULL;
    if (head->email == NULL) return;
    struct list *prev = head;
    struct list *next = head->next;
    while (next->next != NULL){
        prev = next;
        next = next->next;
    }
    prev->next = NULL;
}
void pop_front(struct list **phead){
    struct list *head = *phead;
    if (head->next == NULL) head->email = NULL;
    if (head->email == NULL) return;
    *phead = (*phead)->next;
}
void dump(struct list *head, FILE *f){
    while (head != NULL){
        fprintf(f, "%p %p %s\n", head, head->next, head->email);
        head = head->next;
    }
}

// Создание листа из файла
struct list* generate_from_file(FILE *f){
    struct list *head = create_node();
    unsigned short current_len = 0;
    unsigned short index, len;
    char email[__UINT16_MAX__];
    while (1){
        memset(email, 0, sizeof(char)*__UINT16_MAX__);
        size_t result = fread(&index, sizeof(unsigned short), 1, f);
        if (result != 1){
            break;
        }
        fread(&len, sizeof(unsigned short), 1, f);
        fread(&email, sizeof(char), len, f);
        if (index+1 > current_len){
            extension_list(head, index+1);
            current_len = index+1;
        }
        set_email(head, index, email);
    }
    return head;
}
// Выполнение команды command
exit_code eval(struct list **phead, char* command){

    struct list *head = *phead;
    char *word = strtok(command, " ");
    if (word == NULL){
        return -1;
    }
    if (strcmp(word, "push_front") == 0){
        word = strtok(NULL, " ");
        if (word == NULL){
            fprintf(stderr, "Usage: push_front строка1 [строка2 ... строкаN]\n");
            return 4;
        }
        struct list *newHead = create_node();
        while (word != NULL){
            if (check_email(word) == 1){
                struct list *node = create_node();
                node->email = (char *)malloc(strlen(word) + 1);
                strcpy(node->email, word);
                push_back(newHead, node);
            }
            else{
                fprintf(stderr, "%s - не почта\n", word);

            }
            word = strtok(NULL, " ");
        }
        push_back(newHead, head);
        *phead = newHead;
    }
    else if (strcmp(word, "push_back") == 0){
        word = strtok(NULL, " ");
        if (word == NULL){
            fprintf(stderr, "Usage: push_back строка1 [строка2 ... строкаN]\n");
            return 4;
        }
        while (word != NULL){
            if (check_email(word) == 1){
                struct list *node = create_node();
                node->email = (char *)malloc(strlen(word) + 1);
                strcpy(node->email, word);
                push_back(head, node);
            }
            else{
                fprintf(stderr, "%s - не почта\n", word);
            }
            word = strtok(NULL, " ");
        }
    }
    else if (strcmp(word, "pop_front") == 0){
        word = strtok(NULL, " ");
        if (word != NULL){
            fprintf(stderr, "Usage: pop_front\n");
            return 4;
        }
        pop_front(phead);
    }
    else if (strcmp(word, "pop_back") == 0){
        word = strtok(NULL, " ");
        if (word != NULL){
            fprintf(stderr, "Usage: pop_back\n");
            return 4;
        }
        pop_back(head);
    }
    else if (strcmp(word, "dump") == 0){
        word = strtok(NULL, " ");
        FILE *f = stdout;
        int close = 0;
        if (word != NULL){
            f = fopen(word, "w");
            close = 1;
            if (f == NULL) {
                fprintf(stderr, "Ошибка открытия файла!\n");
                return 5;
            }
        }
        dump(head, f);
        if (close) fclose(f);
    }
    else if (strcmp(word, "delete_odd") == 0){
        word = strtok(NULL, " ");
        if (word != NULL){
            fprintf(stderr, "Usage: delete_odd\n");
            return 4;
        }
        pop_front(phead);
        struct list *head = *phead;
        while (head->next != NULL){
            if (head->next->next == NULL){
                head->next = NULL;
                continue;
            }
            head->next = head->next->next;
            head = head->next;
        }
    }
    else{
        fprintf(stderr, "Неизвестная комманда: %s\n", word);
        return 3;
    }
    return -1;
}


int main(int argc, char *argv[]) {
    if (argc != 2){
        fprintf(stderr, "Usage: %s имя_файла\n       %s -v\n", argv[0], argv[0]);
        return EXIT_SUCCESS;
    }
    if (strcmp(argv[1], "-v") == 0){
        fprintf(stderr, "Иван Вячеславович Порошин, гр. N3149\nВариант 1-5-4-6\n");
        return EXIT_SUCCESS;}
    
    // Чтение из файла
    FILE *f= fopen(argv[1], "rb");
    if (!f){
        fprintf(stderr, "Ошибка: входного файла не существует\n");
        return 12;
    }
    struct list *head = generate_from_file(f);
    fclose(f);
    

    // Обработка команд
    char* text = calloc(1024, sizeof(char));
    if (text == NULL){
        fprintf(stderr, "Ошибка: мало места\n");
        return EXIT_FAILURE;
    }
    char c;
    unsigned long long i;
    exit_code exitCode;
    do{
        i = 0;
        do{
            c = getc(stdin);
            text[i] = c;
            i++;
            if (i == strlen(text)){
                text = realloc(text, i*2*sizeof(char));
                if (text == NULL){
                    fprintf(stderr, "Ошибка: мало места или слишком большой текст\n");
                    return EXIT_FAILURE;
                }
            }
        }while (c != '\n' && c != EOF);
        if (c != EOF){
            text[i-1] = '\0';
            exitCode = eval(&head, text);
            if (exitCode != -1) return exitCode;
        }
    }while (c != EOF);
    
    // Запись в файл
    f = fopen(argv[1], "wb");
    if (head->email == NULL){
        return EXIT_SUCCESS;
    }
    unsigned short j = 0;
    size_t len;
    while (head != NULL){
        len = strlen(head->email);
        fwrite(&j, sizeof(unsigned short), 1, f);
        fwrite(&len, sizeof(unsigned short), 1, f);
        fwrite(head->email, sizeof(char), len, f);
        head = head->next;
        j++;
    }
    fclose(f);
    return EXIT_SUCCESS;
}
