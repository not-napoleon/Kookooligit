CC = clang
CFLAGS = -I. -Ilib/libfov-1.0.4/fov/ -Ilib/mtwist-1.5/ `sdl2-config --cflags` -g
PYTHON = /usr/bin/env python
OBJECTS = \
					creature.o\
					creature_type.o\
					messages.o\
				 	SDL_Tools.o\
				 	command.o\
				 	command_list.o\
				 	draw_map.o\
				 	fov.o\
				 	game.o\
				 	graphics_wrapper.o\
				 	init.o\
				 	look.o\
				 	map.o\
				 	map_section.o\
				 	player.o\
				 	random.o\
				 	render_text.o\
				 	sprite.o\
				 	tile.o\

GENERATED_CODE_DIR = ./generated/
ALL_DEP = log.h

game.out: main.c $(OBJECTS) $(ALL_DEP)
	$(CC) main.c $(OBJECTS) -I. `sdl2-config --cflags --libs` -lSDL2_ttf -o game.out -g -v

SDL_Tools.o: SDL_Tools.c SDL_Tools.h graphics_wrapper.h $(ALL_DEP)
command.o: command.c command.h lib/uthash/src/uthash.h $(ALL_DEP)
creature.o: creature.c creature.h creature_type.h $(ALL_DEP)
creature_type.o: creature_type.c creature_type.h $(ALL_DEP)
draw_map.o: draw_map.c draw_map.h graphics_wrapper.h map.h sprite.h $(ALL_DEP)
game.o: game.c game.h messages.h map.h command.h command_list.h tile.h look.h player.h $(ALL_DEP)
graphics_wrapper.o: graphics_wrapper.c graphics_wrapper.h SDL_Tools.h $(ALL_DEP)
init.o: init.c init.h messages.h map.h game.h draw_map.h tile.h graphics_wrapper.h sprite.h command.h random.h SDL_Tools.h creature_type.h $(ALL_DEP)
look.o: look.c look.h render_text.h color_palette.h $(ALL_DEP)
map.o: map.c map.h tile.h $(ALL_DEP)
map_section.o: map_section.c map_section.h tile.h random.h point.h $(ALL_DEP)
messages.o: messages.c messages.h render_text.h $(ALL_DEP)
player.o: player.c player.h $(ALL_DEP)
render_text.o: render_text.c render_text.h messages.h $(ALL_DEP)
sprite.o: sprite.c sprite.h color_palette.h graphics_wrapper.h SDL_tools.h $(ALL_DEP)
tile.o: tile.c tile.h $(ALL_DEP)

$(GENERATED_CODE_DIR):
	echo "Trying to make dir for generated code"
	mkdir $(GENERATED_CODE_DIR)

command_list.o: $(GENERATED_CODE_PATH)command_list.c command_list.h $(ALL_DEP)
	$(CC) $(CFLAGS) -c -o command_list.o $(GENERATED_CODE_DIR)command_list.c

$(GENERATED_CODE_PATH)command_list.c: $(GENERATED_CODE_DIR) command_list.h build_tools/string_to_enum.py $(ALL_DEP)
	$(PYTHON) build_tools/string_to_enum.py command_list.h string_to_command_code $(GENERATED_CODE_DIR)

fov.o: lib/libfov-1.0.4/fov/fov.c lib/libfov-1.0.4/fov/fov.h
	$(CC) $(CFLAGS) -c -o fov.o lib/libfov-1.0.4/fov/fov.c

random.o: random.c random.h lib/mtwist-1.5/mtwist.c lib/mtwist-1.5/mtwist.h
	$(CC) $(CFLAGS) -c -o random.o random.c

clean:
	rm $(OBJECTS)
	rm -r $(GENERATED_CODE_DIR)
	rm game.out
