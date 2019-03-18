CC = clang
CFLAGS = -Ilib/libfov-1.0.4/fov/ -Ilib/mtwist-1.5/ -I./include/ `sdl2-config --cflags` -g
PYTHON = /usr/bin/env python2
OBJECTS = \
					combat.o\
					creature.o\
					creature_type.o\
					messages.o\
					move.o\
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
SRC = ./src/
INC = ./include/
ALL_DEP = $(INC)log.h

game.out: $(SRC)main.c $(OBJECTS) $(ALL_DEP)
	$(CC) -I$(INC) $(SRC)main.c $(OBJECTS) `sdl2-config --cflags --libs` -lSDL2_ttf -o game.out -g -v

SDL_Tools.o: $(SRC)SDL_Tools.c $(INC)SDL_Tools.h $(INC)graphics_wrapper.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
combat.o: $(SRC)combat.c $(INC)combat.h $(INC)creature.h $(INC)messages.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
command.o: $(SRC)command.c $(INC)command.h lib/uthash/src/uthash.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
creature.o: $(SRC)creature.c $(INC)creature.h $(INC)creature_type.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
creature_type.o: $(SRC)creature_type.c $(INC)creature_type.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
draw_map.o: $(SRC)draw_map.c $(INC)draw_map.h $(INC)graphics_wrapper.h $(INC)map.h $(INC)sprite.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
game.o: $(SRC)game.c $(INC)game.h $(INC)messages.h $(INC)map.h $(INC)command.h $(INC)command_list.h $(INC)tile.h $(INC)look.h $(INC)player.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
graphics_wrapper.o: $(SRC)graphics_wrapper.c $(INC)graphics_wrapper.h $(INC)SDL_Tools.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
init.o: $(SRC)init.c $(INC)init.h $(INC)messages.h $(INC)map.h $(INC)game.h $(INC)draw_map.h $(INC)tile.h $(INC)graphics_wrapper.h $(INC)sprite.h $(INC)command.h $(INC)random.h $(INC)SDL_Tools.h $(INC)creature_type.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
look.o: $(SRC)look.c $(INC)look.h $(INC)render_text.h $(INC)color_palette.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
map.o: $(SRC)map.c $(INC)map.h $(INC)tile.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
map_section.o: $(SRC)map_section.c $(INC)map_section.h $(INC)tile.h $(INC)random.h $(INC)point.h $(INC)creature.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
move.o: $(SRC)move.c $(INC)move.h $(INC)combat.h $(INC)creature.h $(INC)messages.h $(INC)map.h $(INC)player.h $(INC)tile.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
messages.o: $(SRC)messages.c $(INC)messages.h $(INC)render_text.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
player.o: $(SRC)player.c $(INC)player.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
render_text.o: $(SRC)render_text.c $(INC)render_text.h $(INC)messages.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
sprite.o: $(SRC)sprite.c $(INC)sprite.h $(INC)color_palette.h $(INC)graphics_wrapper.h $(INC)SDL_tools.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
tile.o: $(SRC)tile.c $(INC)tile.h $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@
random.o: $(SRC)random.c $(INC)random.h lib/mt19937ar.c $(ALL_DEP)
	$(CC) -I. $(CFLAGS) -c $< -o $@

$(GENERATED_CODE_DIR):
	echo "Trying to make dir for generated code"
	mkdir $(GENERATED_CODE_DIR)

command_list.o: $(GENERATED_CODE_PATH)command_list.c $(INC)command_list.h $(ALL_DEP)
	$(CC) $(CFLAGS) -c -o command_list.o $(GENERATED_CODE_DIR)command_list.c

$(GENERATED_CODE_PATH)command_list.c: $(GENERATED_CODE_DIR) $(INC)command_list.h build_tools/string_to_enum.py $(ALL_DEP)
	$(PYTHON) build_tools/string_to_enum.py $(INC)command_list.h string_to_command_code $(GENERATED_CODE_DIR)

fov.o: lib/libfov-1.0.4/fov/fov.c lib/libfov-1.0.4/fov/fov.h
	$(CC) $(CFLAGS) -c -o fov.o lib/libfov-1.0.4/fov/fov.c


clean:
	rm $(OBJECTS)
	rm -r $(GENERATED_CODE_DIR)
	rm game.out
