CC = gcc -I. `sdl-config --cflags`
OBJECTS = messages.o
game.out: main.c $(OBJECTS)
	gcc main.c $(OBJECTS) -I. `sdl-config --cflags --libs` -lSDL_ttf -o game.out

messages.o: messages.c messages.h

clean:
	rm $(OBJECTS)
