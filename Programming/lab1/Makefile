.PHONY: all clean debug

APP=prg1ivpN3149
CFLAGS=-Wall -Wextra -Wno-implicit-fallthrough

all: $(APP)

$(APP): $(APP).c
	gcc -o $(APP) $(CFLAGS) $(APP).c

debug: $(APP).c
	gcc -o $(APP) $(CFLAGS) -g $(APP).c

clean:
	rm $(APP)
