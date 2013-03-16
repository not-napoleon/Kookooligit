game.out: main.c
	gcc main.c `sdl-config --cflags --libs` -lSDL_ttf -o game.out
