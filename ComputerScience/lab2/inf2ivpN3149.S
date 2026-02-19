global main

section .bss
random_ull resq 1
random_word resb 2

section .data
n dq 0
debuginfo dq 0
loop_itr dq 512
sdvig dq 9223372036854775808 ; 2^63

sys_write equ 1
sys_read equ 0


usage db "Usage: ./inf2ivpN3149 [неотрицательное целое число меньшее чем 2^64]", 10, 0
guess db "Дайте угадаю, ваше число:", 10, 0
confirm db "Сейчас тетрады вашего числа поменяются случайным образом.", 10, "Вы хотите видеть какие тетрады меняются?(y/something)", 10, 0
swap db "swap ", 0


space db " ", 0
newline db "", 10, 0

zero db "0", 0
one db "1", 0
two db "2", 0
three db "3", 0
four db "4", 0
five db "5", 0
six db "6", 0
seven db "7", 0
eight db "8", 0
nine db "9", 0
ten db "10", 0
eleven db "11", 0
twelve db "12", 0
thirteen db "13", 0
fourteen db "14", 0
fifteen db "15", 0
sixteen db "16", 0

buffer db 1




section .text
main:
    cmp rdi, 1
    jz rand
    cmp rdi, 2
    jz norand

    error:
        mov rax, sys_write
        mov rdi, 1
        mov rsi, usage
        call str_len
        syscall

        mov rax, 60
        syscall

    ; is_valid_ull
    norand:
        mov rsi, [rsi+8]
        mov rax, 0
        mov rbx, 10
        mov rdi, 1844674407370955161

        char_loop:
        movzx rcx, byte [rsi]
        mov [n], rax
        test rcx, rcx
        jz base
        sub rcx, '0'
        cmp rcx, 9
        ja error

        cmp rax, rdi
        ja error
        jnz last_check_skip

        cmp rcx, 5
        ja error

        last_check_skip:

        mul rbx
        add rax, rcx
        jc error

        inc rsi
        jmp char_loop

    
    rand:
        call generate_rand_n

        mov rax, sys_write
        mov rdi, 1
        mov rsi, guess
        call str_len
        syscall


    base:
    call printbin

    mov rax, sys_write
    mov rdi, 1
    mov rsi, confirm
    call str_len
    syscall

    mov rax, sys_read
    mov rdi, 0
    lea rsi, [buffer]
    mov rdx, 1
    syscall

    mov rdi, 'y'
    cmp [buffer], rdi
    jnz loop_main

    mov rdi, 1
    mov [debuginfo], rdi

    loop_main:
        mov rax, 318
        lea rdi, [random_word]
        mov rsi, 2
        mov rdx, 0
        syscall

        mov al, [random_word]
        mov bl, [random_word + 1]

        shr al, 4
        shr bl, 4
        shl al, 2
        shl bl, 2
        ; в al и bl лежат смщения двух тетрад, которые мы будем менять местами

        mov rdi, 15
        mov cl, al

        shl rdi, cl
        and rdi, [n]
        xor [n], rdi
        shr rdi, cl
        ; обнуляем первую тетраду, сохраняя её значение в rdi


        mov rdx, 15
        mov cl, bl

        shl rdx, cl
        and rdx, [n]
        xor [n], rdx
        shr rdx, cl
        ; обнуляем вторую тетраду, сохраняя её значение в rdx

        shl rdi, cl
        xor [n], rdi
        ; записываем в вторую тетраду значение первой

        mov cl, al

        shl rdx, cl
        xor [n], rdx
        ; записываем в первую тетраду значение второй

        mov rdi, 0
        cmp [debuginfo], rdi
        jz loop_end
        shr al, 2
        shr bl, 2
        mov r13b, al

        mov rax, sys_write
        mov rdi, 1
        mov rsi, swap
        call str_len
        syscall
        
        mov al, r13b
        call int_to_str
        mov rax, sys_write
        mov rdi, 1
        call str_len
        syscall

        mov rax, sys_write
        mov rdi, 1
        mov rsi, space
        call str_len
        syscall

        mov al, bl
        call int_to_str
        mov rax, sys_write
        mov rdi, 1
        call str_len
        syscall

        mov rax, sys_write
        mov rdi, 1
        mov rsi, newline
        call str_len
        syscall

        call printbin
        loop_end:
        mov r11, [loop_itr]
        dec r11
        mov [loop_itr], r11
        cmp r11, 0
        jnz loop_main

    
    call printbin

    mov rax, 60
    mov rdi, 0
    syscall


printbin:

    mov r10, 1
    
    loop_printbin:

    mov rsi, zero

    mov rdi, [sdvig]
    and rdi, [n]
    jz zero_printbin
    
    mov rsi, one

    zero_printbin:

    mov rax, sys_write
    mov rdi, 1
    call str_len
    syscall

    mov r9, r10
    and r9, 7
    cmp r9, 0
    jnz endloop_printbin

    mov rax, sys_write
    mov rdi, 1
    mov rsi, space
    call str_len
    syscall

    endloop_printbin:
    shr qword [sdvig], 1
    inc r10
    cmp qword [sdvig], 0
    jnz loop_printbin

    mov rax, sys_write
    mov rdi, 1
    mov rsi, newline
    call str_len
    syscall

    mov r10, 9223372036854775808
    mov qword [sdvig], r10
    ret




    
generate_rand_n:
    mov rax, 318
    lea rdi, [random_ull]
    mov rsi, 8
    mov rdx, 0
    syscall

    mov rsi, [random_ull]
    mov qword [n], rsi
    ret

str_len:
        xor rdx, rdx
    .len_loop:
        cmp byte [rsi + rdx], 0
        je .done
        inc rdx
        jmp .len_loop
    .done:
        ret


int_to_str:
    cmp al, 1
    jnz m2
    mov rsi, one
    ret

    m2:
    cmp al, 2
    jnz m3
    mov rsi, two
    ret

    m3:
    cmp al, 3
    jnz m4
    mov rsi, three
    ret

    m4:
    cmp al, 4
    jnz m5
    mov rsi, four
    ret

    m5:
    cmp al, 5
    jnz m6
    mov rsi, five
    ret

    m6:
    cmp al, 6
    jnz m7
    mov rsi, six
    ret

    m7:
    cmp al, 7
    jnz m8
    mov rsi, seven
    ret

    m8:
    cmp al, 8
    jnz m9
    mov rsi, eight
    ret

    m9:
    cmp al, 9
    jnz m10
    mov rsi, nine
    ret

    m10:
    cmp al, 10
    jnz m11
    mov rsi, ten
    ret

    m11:
    cmp al, 11
    jnz m12
    mov rsi, eleven
    ret

    m12:
    cmp al, 12
    jnz m13
    mov rsi, twelve
    ret

    m13:
    cmp al, 13
    jnz m14
    mov rsi, thirteen
    ret

    m14:
    cmp al, 14
    jnz m15
    mov rsi, fourteen
    ret

    m15:
    cmp al, 15
    jnz m16
    mov rsi, fifteen
    ret

    m16:
    mov rsi, sixteen
    ret
