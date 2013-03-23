CC = gcc -I. `sdl-config --cflags` -g
OBJECTS = messages.o init.o
game.out: main.c $(OBJECTS)
	gcc main.c $(OBJECTS) -I. `sdl-config --cflags --libs` -lSDL_ttf -o game.out -g

messages.o: messages.c messages.h
init.o: init.c init.h

clean:
	rm $(OBJECTS)
	rm game.out
