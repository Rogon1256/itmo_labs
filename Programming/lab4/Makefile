.PHONY: all clean test gen

APP=prg4ivpN3149
CFLAGS=-Wall -Wextra -Werror -g

all: $(APP)

$(APP): $(APP).c
	gcc -o $(APP) $(CFLAGS) $(APP).c

test: $(APP)
	python3 $(APP)_gen.py test.data
	./$(APP) test.data

gen:
	python3 $(APP)_gen.py test.data

clean:
	rm $(APP)

