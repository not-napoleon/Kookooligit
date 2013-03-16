game: main.c
	gcc main.c `sdl-config --cflags --libs` -o game
