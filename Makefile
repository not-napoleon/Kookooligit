CC = gcc 
CFLAGS = -I. -Ilib/libfov-1.0.4/fov/ `sdl-config --cflags` -g
OBJECTS = messages.o init.o map.o draw_map.o fov.o command.o game.o render_text.o look.o tile.o
ALL_DEP = log.h

game.out: main.c $(OBJECTS) $(ALL_DEP)
	gcc main.c $(OBJECTS) -I. `sdl-config --cflags --libs` -lSDL_ttf -o game.out -g

command.o: command.c command.h $(ALL_DEP)
draw_map.o: draw_map.c draw_map.h map.h color_palette.h tile.h $(ALL_DEP)
game.o: game.c game.h messages.h map.h command.h tile.h $(ALL_DEP)
init.o: init.c init.h messages.h map.h game.h $(ALL_DEP)
look.o: look.c look.h render_text.h color_palette.h $(ALL_DEP)
map.o: map.c map.h tile.h $(ALL_DEP)
messages.o: messages.c messages.h render_text.h $(ALL_DEP)
render_text.o: render_text.c render_text.h messages.h $(ALL_DEP)
tile.o: tile.c tile.h $(ALL_DEP)

fov.o: lib/libfov-1.0.4/fov/fov.c lib/libfov-1.0.4/fov/fov.h
	$(CC) $(CFLAGS) -c -o fov.o lib/libfov-1.0.4/fov/fov.c

clean:
	rm $(OBJECTS)
	rm game.out
