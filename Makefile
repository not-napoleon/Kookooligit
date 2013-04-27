CC = gcc 
CFLAGS = -I. -Ilib/libfov-1.0.4/fov/ `sdl-config --cflags` -g
OBJECTS = messages.o init.o map.o draw_map.o fov.o command.o game.o
ALL_DEP = Makefile log.h

game.out: main.c $(OBJECTS) $(ALL_DEP)
	gcc main.c $(OBJECTS) -I. `sdl-config --cflags --libs` -lSDL_ttf -o game.out -g

command.o: command.c command.h $(ALL_DEP)
draw_map.o: draw_map.c draw_map.h map.h color_palette.h $(ALL_DEP)
game.o: game.c game.h messages.h map.h command.h $(ALL_DEP)
init.o: init.c init.h messages.h map.h game.h $(ALL_DEP)
map.o: map.c map.h $(ALL_DEP)
messages.o: messages.c messages.h $(ALL_DEP)

fov.o: lib/libfov-1.0.4/fov/fov.c lib/libfov-1.0.4/fov/fov.h
	$(CC) $(CFLAGS) -c -o fov.o lib/libfov-1.0.4/fov/fov.c

clean:
	rm $(OBJECTS)
	rm game.out
