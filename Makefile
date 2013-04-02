CC = gcc 
CFLAGS = -I. -Ilib/libfov-1.0.4/fov/ `sdl-config --cflags` -g
OBJECTS = messages.o init.o map.o draw_map.o fov.o
game.out: main.c $(OBJECTS)
	gcc main.c $(OBJECTS) -I. `sdl-config --cflags --libs` -lSDL_ttf -o game.out -g

messages.o: messages.c messages.h
init.o: init.c init.h messages.h map.h
map.o: map.c map.h
draw_map.o: draw_map.c draw_map.h map.h
fov.o: lib/libfov-1.0.4/fov/fov.c lib/libfov-1.0.4/fov/fov.h
	$(CC) $(CFLAGS) -c -o fov.o lib/libfov-1.0.4/fov/fov.c

clean:
	rm $(OBJECTS)
	rm game.out
