@all:
	@gcc main.c src/*.c -I/usr/include/SDL/ -Iinclude/ -lSDL_image -lSDL_mixer -lSDL -o soko -ansi -Wall

clean:
	@rm -f soko soko*.exe
