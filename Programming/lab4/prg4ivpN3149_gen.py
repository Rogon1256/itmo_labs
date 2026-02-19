#!/usr/bin/python3

import sys, random

alfabet = "abcdefghijklmnopqrstuvwxyz"

def rand_email() -> str:
    string = ""
    for i in range(3):
        for j in range(random.randint(5, 12)):
            string += random.choice(alfabet)
        if i == 0:
            string += "@"
        if i == 1:
            string += "."
    return string



def generate_bytes(n:int) -> bytes:
    l = []
    for i in range(n):
        email = rand_email()
        l.append((i , len(email), email))
    random.shuffle(l)
    bindata = b''
    for i in l:
        print(f"i={i[0]} len={i[1]} email={i[2]}")
        bindata += (i[0]).to_bytes(2, "little") + (i[1]).to_bytes(2, "little") + i[2].encode()
    return bindata


def main():
    if len(sys.argv) == 1:
        print(f"Usage: {sys.argv[0]} -v\n       {sys.argv[0]} [-n N] имя_файла")
        exit(0)
    
    if len(sys.argv) == 2:
        if sys.argv[1] == "-v":
            print("Иван Вячеславович Порошин, гр. N3149\nВариант 1-5-4-6")
            exit(0)
        path = sys.argv[1]
        bindata = generate_bytes(random.randint(10, 1000))
        
    if len(sys.argv) == 4:
        if sys.argv[1] != "-n":
            print("Неверное число аргументов")
            exit(1)
        if sys.argv[2].isdigit() == False:
            print(f"{sys.argv[2]} не число")
            exit(2)
        path = sys.argv[3]
        bindata = generate_bytes(int(sys.argv[2]))
    
    with open(path, "wb") as f:
        f.write(bindata)
if __name__ == '__main__':
    main()

