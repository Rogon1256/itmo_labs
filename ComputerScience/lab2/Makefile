.PHONY: all clean debug

APP=inf2ivpN3149
CFLAGS=-Wall -Wextra -Werror -Wno-implicit-fallthrough

all: $(APP)

$(APP): $(APP).c $(APP).S
	gcc -o $(APP)_c $(CFLAGS) $(APP).c
	nasm -felf64 $(APP).S
	gcc -no-pie -o $(APP)_n $(APP).o

clean:
	rm -rf $(APP)_c $(APP)_n $(APP).o
