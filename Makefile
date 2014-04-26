CC = clang
CFLAGS = -I. -Ilib/libfov-1.0.4/fov/ -Ilib/mtwist-1.5/ `sdl2-config --cflags` -g
OBJECTS = messages.o init.o map.o draw_map.o fov.o command.o game.o render_text.o look.o tile.o graphics_wrapper.o SDL_Tools.o random.o map_section.o
ALL_DEP = log.h

game.out: main.c $(OBJECTS) $(ALL_DEP)
	$(CC) main.c $(OBJECTS) -I. `sdl2-config --cflags --libs` -lSDL_ttf -o game.out -g -v

SDL_Tools.o: SDL_Tools.c SDL_Tools.h $(ALL_DEP)
command.o: command.c command.h $(ALL_DEP)
draw_map.o: draw_map.c draw_map.h map.h color_palette.h tile.h $(ALL_DEP)
game.o: game.c game.h messages.h map.h command.h tile.h look.h $(ALL_DEP)
graphics_wrapper.o: graphics_wrapper.c graphics_wrapper.h $(ALL_DEP)
init.o: init.c init.h messages.h map.h game.h draw_map.h tile.h $(ALL_DEP)
look.o: look.c look.h render_text.h color_palette.h $(ALL_DEP)
map.o: map.c map.h tile.h $(ALL_DEP)
map_section.o: map_section.c map_section.h tile.h random.h $(ALL_DEP)
messages.o: messages.c messages.h render_text.h $(ALL_DEP)
render_text.o: render_text.c render_text.h messages.h $(ALL_DEP)
tile.o: tile.c tile.h $(ALL_DEP)

fov.o: lib/libfov-1.0.4/fov/fov.c lib/libfov-1.0.4/fov/fov.h
	$(CC) $(CFLAGS) -c -o fov.o lib/libfov-1.0.4/fov/fov.c

random.o: random.c random.h lib/mtwist-1.5/mtwist.c lib/mtwist-1.5/mtwist.h
	$(CC) $(CFLAGS) -c -o random.o random.c 

clean:
	rm $(OBJECTS)
	rm game.out
