all:
	gcc -Iinclude -Iinclude/sdl -Iinclude/headers -o poing main.c `sdl2-config --libs`
	./poing